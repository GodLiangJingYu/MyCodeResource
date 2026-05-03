"""
S-ONNXCompiler - Parser (语法分析器)
基于递归下降的自上而下语法分析，根据S-ONNX文法规则构建AST
"""

from lexer import TokenType, Token
from ast_nodes import *


class ParseError(Exception):
    """语法错误"""
    def __init__(self, message: str, line: int = 0, column: int = 0, snippet: str = ""):
        self.message = message
        self.line = line
        self.column = column
        self.snippet = snippet
        super().__init__(self.format_error())

    def format_error(self):
        msg = f"[语法错误] 第{self.line}行 第{self.column}列: {self.message}"
        if self.snippet:
            msg += f"\n  代码片段: {self.snippet}"
        return msg


class Parser:
    """
    S-ONNX语法分析器
    采用递归下降分析法（自上而下）
    """

    def __init__(self, tokens):
        self.tokens = [t for t in tokens if t.token_type != TokenType.ERROR]
        self.pos = 0
        self.errors = []

    def _current(self) -> Token:
        """获取当前Token"""
        if self.pos < len(self.tokens):
            return self.tokens[self.pos]
        return self.tokens[-1]  # EOF

    def _peek(self, offset=1) -> Token:
        """向前看offset个Token"""
        idx = self.pos + offset
        if idx < len(self.tokens):
            return self.tokens[idx]
        return self.tokens[-1]

    def _advance(self) -> Token:
        """消费当前Token并前进"""
        tok = self._current()
        if self.pos < len(self.tokens) - 1:
            self.pos += 1
        return tok

    def _match(self, expected: TokenType) -> Token:
        """匹配指定类型的Token，成功则消费，否则报错"""
        tok = self._current()
        if tok.token_type == expected:
            return self._advance()
        else:
            err = ParseError(
                f"期望 {expected.name}，但得到 {tok.token_type.name}('{tok.value}')",
                tok.line, tok.column
            )
            self.errors.append(err)
            raise err

    def _match_kw(self, keyword: str) -> Token:
        """匹配关键字"""
        kw_type = None
        for k, v in {
            "ModelProto": TokenType.KW_MODELPROTO,
            "graph": TokenType.KW_GRAPH,
            "name": TokenType.KW_NAME,
            "node": TokenType.KW_NODE,
            "input": TokenType.KW_INPUT,
            "output": TokenType.KW_OUTPUT,
            "op_type": TokenType.KW_OP_TYPE,
            "attribute": TokenType.KW_ATTRIBUTE,
            "initializer": TokenType.KW_INITIALIZER,
            "doc_string": TokenType.KW_DOC_STRING,
            "domain": TokenType.KW_DOMAIN,
            "model_version": TokenType.KW_MODEL_VERSION,
            "producer_name": TokenType.KW_PRODUCER_NAME,
            "producer_version": TokenType.KW_PRODUCER_VERSION,
            "type": TokenType.KW_TYPE,
            "tensor_type": TokenType.KW_TENSOR_TYPE,
            "ir_version": TokenType.KW_IR_VERSION,
            "elem_type": TokenType.KW_ELEM_TYPE,
            "shape": TokenType.KW_SHAPE,
            "dim": TokenType.KW_DIM,
            "dims": TokenType.KW_DIMS,
            "raw_data": TokenType.KW_RAW_DATA,
            "opset_import": TokenType.KW_OPSET_IMPORT,
            "dim_value": TokenType.KW_DIM_VALUE,
            "dim_param": TokenType.KW_DIM_PARAM,
            "data_type": TokenType.KW_DATA_TYPE,
            "version": TokenType.KW_VERSION,
            "value": TokenType.KW_VALUE,
            "int": TokenType.KW_INT,
            "float": TokenType.KW_FLOAT,
            "string": TokenType.KW_STRING,
            "bool": TokenType.KW_BOOL,
        }.items():
            if k.lower() == keyword.lower():
                kw_type = v
                break
        if kw_type is None:
            raise ValueError(f"未知关键字: {keyword}")
        return self._match(kw_type)

    def _try_match(self, expected: TokenType) -> bool:
        """尝试匹配，不消费"""
        return self._current().token_type == expected

    def _parse_string_value(self) -> str:
        """解析字符串字面量的值（去掉引号，处理转义）"""
        tok = self._match(TokenType.STRING_LIT)
        s = tok.value
        # 去掉首尾引号
        if s.startswith('"') and s.endswith('"'):
            s = s[1:-1]
        # 处理转义
        s = s.replace('\\b', '\b').replace('\\t', '\t').replace('\\n', '\n')
        s = s.replace('\\f', '\f').replace('\\r', '\r')
        s = s.replace('\\"', '"').replace("\\'", "'").replace('\\\\', '\\')
        return s

    def _parse_integer_value(self) -> str:
        """解析整数字面量的值"""
        tok = self._match(TokenType.INTEGER)
        val = tok.value
        # 去掉类型后缀
        if val and val[-1] in ('l', 'L'):
            val = val[:-1]
        return val

    def _parse_bytes_value(self) -> str:
        """解析字节数据的值"""
        tok = self._match(TokenType.BYTES)
        val = tok.value
        if val and val[-1] in ('b', 'B'):
            val = val[:-1]
        return val

    # ========== 文法规则解析 ==========

    def parse(self) -> ModelProtoNode:
        """解析入口: model -> "ModelProto" "{" model_body_def "}" """
        try:
            self._match_kw("ModelProto")
            self._match(TokenType.LBRACE)
            model = self._parse_model_body_def()
            self._match(TokenType.RBRACE)

            # 检查是否还有多余的Token（如测试用例7的 keyword = "value"）
            if not self._try_match(TokenType.EOF):
                tok = self._current()
                err = ParseError(
                    f"模型定义结束后存在多余内容: '{tok.value}'",
                    tok.line, tok.column
                )
                self.errors.append(err)

            return model
        except ParseError:
            # 尝试错误恢复
            return self._error_recovery_model()

    def _parse_model_body_def(self) -> ModelProtoNode:
        """
        model_body_def -> ir_version_def producer_name_def [producer_version_def]
                          [domain_def] [model_version_def] [doc_string_def]
                          graph_def [opset_import_def]
        部分字段在实际测试用例中可能缺失，使用可选解析增强容错性
        """
        model = ModelProtoNode()

        # ir_version_def -> "ir_version" "=" INTEGER（必需）
        if self._try_match(TokenType.KW_IR_VERSION):
            self._match_kw("ir_version")
            self._match(TokenType.ASSIGN)
            model.ir_version = self._parse_integer_value()
        else:
            model.ir_version = "0"
            self.errors.append(ParseError("缺少ir_version定义", self._current().line, self._current().column))

        # producer_name_def -> "producer_name" "=" STRING（必需）
        if self._try_match(TokenType.KW_PRODUCER_NAME):
            self._match_kw("producer_name")
            self._match(TokenType.ASSIGN)
            model.producer_name = self._parse_string_value()
        else:
            model.producer_name = ""
            self.errors.append(ParseError("缺少producer_name定义", self._current().line, self._current().column))

        # producer_version_def -> "producer_version" "=" STRING（可选）
        if self._try_match(TokenType.KW_PRODUCER_VERSION):
            self._match_kw("producer_version")
            self._match(TokenType.ASSIGN)
            model.producer_version = self._parse_string_value()
        else:
            model.producer_version = ""

        # domain_def -> "domain" "=" STRING（可选）
        if self._try_match(TokenType.KW_DOMAIN):
            self._match_kw("domain")
            self._match(TokenType.ASSIGN)
            model.domain = self._parse_string_value()
        else:
            model.domain = ""

        # model_version_def -> "model_version" "=" INTEGER（可选）
        if self._try_match(TokenType.KW_MODEL_VERSION):
            self._match_kw("model_version")
            self._match(TokenType.ASSIGN)
            model.model_version = self._parse_integer_value()
        else:
            model.model_version = "0"

        # doc_string_def -> "doc_string" "=" STRING（可选）
        if self._try_match(TokenType.KW_DOC_STRING):
            self._match_kw("doc_string")
            self._match(TokenType.ASSIGN)
            model.doc_string = self._parse_string_value()
        else:
            model.doc_string = ""

        # graph_def -> "graph" "{" graph_body_def "}"（必需）
        if self._try_match(TokenType.KW_GRAPH):
            model.graph = self._parse_graph_def()
        else:
            model.graph = GraphNode()
            self.errors.append(ParseError("缺少graph定义", self._current().line, self._current().column))

        # opset_import_def（可选，缺失时报错但继续）
        if self._try_match(TokenType.KW_OPSET_IMPORT):
            model.opset_import = self._parse_opset_import_def()
        else:
            self.errors.append(ParseError(
                "缺少opset_import定义",
                self._current().line, self._current().column
            ))

        # 检查多余的关键字（如测试用例7的 keyword = "value"）
        while self._try_match(TokenType.IDENTIFIER):
            tok = self._current()
            err = ParseError(
                f"'{tok.value}' 不是合法的S-ONNX关键字",
                tok.line, tok.column
            )
            self.errors.append(err)
            self._advance()
            if self._try_match(TokenType.ASSIGN):
                self._advance()
            if self._try_match(TokenType.STRING_LIT):
                self._advance()
            elif self._try_match(TokenType.INTEGER):
                self._advance()
            elif self._try_match(TokenType.BYTES):
                self._advance()

        return model

    def _parse_graph_def(self) -> GraphNode:
        """graph_def -> "graph" "{" graph_body_def "}" """
        self._match_kw("graph")
        self._match(TokenType.LBRACE)
        graph = self._parse_graph_body_def()
        self._match(TokenType.RBRACE)
        return graph

    def _parse_graph_body_def(self) -> GraphNode:
        """
        graph_body_def -> name_def node_list input_list output_list [initializer_list]
        """
        graph = GraphNode()

        # name_def -> "name" "=" STRING
        self._match_kw("name")
        self._match(TokenType.ASSIGN)
        graph.name = self._parse_string_value()

        # node_list -> node_repeats {node_repeats}
        while self._try_match(TokenType.KW_NODE):
            graph.nodes.append(self._parse_node_repeats())

        # input_list -> input_repeats {input_repeats}
        while self._try_match(TokenType.KW_INPUT):
            graph.inputs.append(self._parse_input_repeats())

        # output_list -> output_repeats {output_repeats}
        while self._try_match(TokenType.KW_OUTPUT):
            graph.outputs.append(self._parse_output_repeats())

        # initializer_list (可选) -> initializer_repeats {initializer_repeats}
        while self._try_match(TokenType.KW_INITIALIZER):
            graph.initializers.append(self._parse_initializer_repeats())

        return graph

    def _parse_node_repeats(self) -> NodeDefNode:
        """node_repeats -> "node" "{" node_def "}" """
        self._match_kw("node")
        self._match(TokenType.LBRACE)
        node_def = self._parse_node_def()
        self._match(TokenType.RBRACE)
        return node_def

    def _parse_node_def(self) -> NodeDefNode:
        """
        node_def -> op_type_def name_def (input_list | input_arr)
                   (output_list | output_arr) [attribute_list]
        """
        node = NodeDefNode()

        # op_type_def -> "op_type" "=" STRING
        self._match_kw("op_type")
        self._match(TokenType.ASSIGN)
        node.op_type = self._parse_string_value()

        # name_def -> "name" "=" STRING
        self._match_kw("name")
        self._match(TokenType.ASSIGN)
        node.name = self._parse_string_value()

        # input_list | input_arr
        if self._try_match(TokenType.KW_INPUT):
            # 判断是input_arr还是input_list
            # input_arr: "input" "=" "[" STRING {"," STRING} "]"
            # input_list: "input" "{" value_info_def "}"
            if self._peek().token_type == TokenType.ASSIGN:
                node.inputs = self._parse_input_arr()
            else:
                node.input_infos.append(self._parse_input_repeats())

        # output_list | output_arr
        if self._try_match(TokenType.KW_OUTPUT):
            if self._peek().token_type == TokenType.ASSIGN:
                node.outputs = self._parse_output_arr()
            else:
                node.output_infos.append(self._parse_output_repeats())

        # attribute_list (可选)
        while self._try_match(TokenType.KW_ATTRIBUTE):
            node.attributes.append(self._parse_attribute_repeats())

        return node

    def _parse_input_arr(self) -> list:
        """input_arr -> "input" "=" "[" STRING { "," STRING } "]" """
        self._match_kw("input")
        self._match(TokenType.ASSIGN)
        self._match(TokenType.LBRACKET)
        inputs = []
        inputs.append(self._parse_string_value())
        while self._try_match(TokenType.COMMA):
            self._match(TokenType.COMMA)
            inputs.append(self._parse_string_value())
        self._match(TokenType.RBRACKET)
        return inputs

    def _parse_output_arr(self) -> list:
        """output_arr -> "output" "=" "[" STRING { "," STRING } "]" """
        self._match_kw("output")
        self._match(TokenType.ASSIGN)
        self._match(TokenType.LBRACKET)
        outputs = []
        outputs.append(self._parse_string_value())
        while self._try_match(TokenType.COMMA):
            self._match(TokenType.COMMA)
            outputs.append(self._parse_string_value())
        self._match(TokenType.RBRACKET)
        return outputs

    def _parse_input_repeats(self) -> InputInfoNode:
        """input_repeats -> "input" "{" value_info_def "}" """
        self._match_kw("input")
        self._match(TokenType.LBRACE)
        info = self._parse_value_info_def()
        info.node_type = "InputInfo"
        self._match(TokenType.RBRACE)
        return info

    def _parse_output_repeats(self) -> OutputInfoNode:
        """output_repeats -> "output" "{" value_info_def "}" """
        self._match_kw("output")
        self._match(TokenType.LBRACE)
        info = self._parse_value_info_def()
        info.node_type = "OutputInfo"
        self._match(TokenType.RBRACE)
        return info

    def _parse_value_info_def(self) -> InputInfoNode:
        """value_info_def -> name_def type_def"""
        info = InputInfoNode()
        self._match_kw("name")
        self._match(TokenType.ASSIGN)
        info.name = self._parse_string_value()
        info.type_info = self._parse_type_def()
        return info

    def _parse_type_def(self) -> TypeInfoNode:
        """type_def -> "type" "{" tensor_type_def "}" """
        self._match_kw("type")
        self._match(TokenType.LBRACE)
        type_info = TypeInfoNode()
        type_info.tensor_type = self._parse_tensor_type_def()
        self._match(TokenType.RBRACE)
        return type_info

    def _parse_tensor_type_def(self) -> TensorTypeNode:
        """tensor_type_def -> "tensor_type" "{" elem_type_def shape_def "}" """
        self._match_kw("tensor_type")
        self._match(TokenType.LBRACE)
        tensor = TensorTypeNode()

        # elem_type_def -> "elem_type" "=" ("int" | "float" | "string" | "bool")
        self._match_kw("elem_type")
        self._match(TokenType.ASSIGN)
        tensor.elem_type = self._parse_data_type()

        # shape_def -> "shape" "{" dim_list "}"
        tensor.shape = self._parse_shape_def()

        self._match(TokenType.RBRACE)
        return tensor

    def _parse_data_type(self) -> str:
        """解析数据类型: int | float | string | bool"""
        tok = self._current()
        if tok.token_type in (TokenType.KW_INT, TokenType.KW_FLOAT,
                              TokenType.KW_STRING, TokenType.KW_BOOL):
            self._advance()
            return tok.value.lower()
        else:
            err = ParseError(
                f"期望数据类型(int/float/string/bool)，但得到 {tok.token_type.name}('{tok.value}')",
                tok.line, tok.column
            )
            self.errors.append(err)
            raise err

    def _parse_shape_def(self) -> ShapeNode:
        """shape_def -> "shape" "{" dim_list "}" """
        self._match_kw("shape")
        self._match(TokenType.LBRACE)
        shape = ShapeNode()
        shape.dims = self._parse_dim_list()
        self._match(TokenType.RBRACE)
        return shape

    def _parse_dim_list(self) -> list:
        """dim_list -> dim_repeats {dim_repeats}"""
        dims = []
        while self._try_match(TokenType.KW_DIM):
            dims.append(self._parse_dim_repeats())
        return dims

    def _parse_dim_repeats(self) -> DimNode:
        """dim_repeats -> "dim" "{" dim_def "}" """
        self._match_kw("dim")
        self._match(TokenType.LBRACE)
        dim = self._parse_dim_def()
        self._match(TokenType.RBRACE)
        return dim

    def _parse_dim_def(self) -> DimNode:
        """dim_def -> ("dim_value" "=" INTEGER) | ("dim_param" "=" STRING)"""
        dim = DimNode()
        if self._try_match(TokenType.KW_DIM_VALUE):
            self._match_kw("dim_value")
            self._match(TokenType.ASSIGN)
            dim.dim_value = self._parse_integer_value()
        elif self._try_match(TokenType.KW_DIM_PARAM):
            self._match_kw("dim_param")
            self._match(TokenType.ASSIGN)
            dim.dim_param = self._parse_string_value()
        else:
            tok = self._current()
            err = ParseError(
                f"期望 dim_value 或 dim_param，但得到 {tok.token_type.name}",
                tok.line, tok.column
            )
            self.errors.append(err)
            raise err
        return dim

    def _parse_attribute_repeats(self) -> AttributeNode:
        """attribute_repeats -> "attribute" "{" attribute_def "}" """
        self._match_kw("attribute")
        self._match(TokenType.LBRACE)
        attr = self._parse_attribute_def()
        self._match(TokenType.RBRACE)
        return attr

    def _parse_attribute_def(self) -> AttributeNode:
        """attribute_def -> name_def value_def"""
        attr = AttributeNode()
        self._match_kw("name")
        self._match(TokenType.ASSIGN)
        attr.name = self._parse_string_value()
        self._match_kw("value")
        self._match(TokenType.ASSIGN)
        attr.value = self._parse_string_value()
        return attr

    def _parse_initializer_repeats(self) -> InitializerNode:
        """initializer_repeats -> "initializer" "{" tensor_def "}" """
        self._match_kw("initializer")
        self._match(TokenType.LBRACE)
        init = self._parse_tensor_def()
        self._match(TokenType.RBRACE)
        return init

    def _parse_tensor_def(self) -> InitializerNode:
        """
        tensor_def -> name_def data_type_def dims_def raw_data_def
        """
        init = InitializerNode()

        # name_def
        self._match_kw("name")
        self._match(TokenType.ASSIGN)
        init.name = self._parse_string_value()

        # data_type_def -> "data_type" "=" ("int" | "float" | "string" | "bool")
        self._match_kw("data_type")
        self._match(TokenType.ASSIGN)
        init.data_type = self._parse_data_type()

        # dims_def -> "dims" "=" INTEGER {INTEGER}
        self._match_kw("dims")
        self._match(TokenType.ASSIGN)
        init.dims.append(self._parse_integer_value())
        while self._try_match(TokenType.INTEGER):
            init.dims.append(self._parse_integer_value())

        # raw_data_def -> "raw_data" "=" BYTES
        self._match_kw("raw_data")
        self._match(TokenType.ASSIGN)
        init.raw_data = self._parse_bytes_value()

        return init

    def _parse_opset_import_def(self) -> OpsetImportNode:
        """
        opset_import_def -> "opset_import" "{" domain_def version_def "}"
        """
        self._match_kw("opset_import")
        self._match(TokenType.LBRACE)
        opset = OpsetImportNode()

        # domain_def -> "domain" "=" STRING
        self._match_kw("domain")
        self._match(TokenType.ASSIGN)
        opset.domain = self._parse_string_value()

        # version_def -> "version" "=" INTEGER
        self._match_kw("version")
        self._match(TokenType.ASSIGN)
        opset.version = self._parse_integer_value()

        self._match(TokenType.RBRACE)
        return opset

    def _error_recovery_model(self) -> ModelProtoNode:
        """错误恢复：尝试构建一个部分模型"""
        model = ModelProtoNode()
        model.ir_version = "0"
        model.producer_name = ""
        model.producer_version = ""
        model.domain = ""
        model.model_version = "0"
        model.doc_string = ""
        model.graph = GraphNode()
        return model

    def get_errors(self):
        """获取语法错误列表"""
        return self.errors
