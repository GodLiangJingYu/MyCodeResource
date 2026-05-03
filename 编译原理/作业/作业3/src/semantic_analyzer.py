"""
S-ONNXCompiler - SemanticAnalyzer (语义分析器)
对S-ONNX模型进行语义检查：
1. 命名冲突（节点名称重复、输出张量名称冲突）
2. 未定义即使用（输入张量必须先定义才能被引用）
3. 张量类型检查（同一操作符输入类型一致、模型输入输出类型匹配）
"""

from ast_nodes import *


class SemanticError:
    """语义错误"""
    def __init__(self, message: str, line: int = 0, column: int = 0, error_type: str = ""):
        self.message = message
        self.line = line
        self.column = column
        self.error_type = error_type  # 错误分类

    def __str__(self):
        prefix = f"[语义错误"
        if self.error_type:
            prefix += f"-{self.error_type}"
        prefix += f"] 第{self.line}行: {self.message}"
        return prefix

    def __repr__(self):
        return str(self)


class SemanticAnalyzer:
    """
    S-ONNX语义分析器
    对AST进行语义检查，检测命名冲突、未定义引用、类型不匹配等问题
    """

    def __init__(self, model: ModelProtoNode):
        self.model = model
        self.errors = []

    def analyze(self) -> list:
        """执行语义分析，返回错误列表"""
        self.errors = []

        if self.model.graph is None:
            self.errors.append(SemanticError(
                "模型缺少graph定义", 0, 0, "结构错误"
            ))
            return self.errors

        # 1. 命名冲突检查
        self._check_naming_conflicts()

        # 2. 未定义即使用检查
        self._check_undefined_references()

        # 3. 张量类型检查
        self._check_tensor_types()

        return self.errors

    def _check_naming_conflicts(self):
        """检查命名冲突：节点名称重复、输出张量名称冲突"""
        graph = self.model.graph

        # 检查节点名称重复
        node_names = {}
        for node in graph.nodes:
            if node.name:
                if node.name in node_names:
                    self.errors.append(SemanticError(
                        f"节点名称重复——同一个模型中，不能有同名节点 '{node.name}'",
                        node.line, node.column, "命名冲突"
                    ))
                else:
                    node_names[node.name] = node

        # 检查输出张量名称冲突
        output_names = {}
        # 从节点输出中检查
        for node in graph.nodes:
            for out_name in node.outputs:
                if out_name in output_names:
                    self.errors.append(SemanticError(
                        f"输出命名冲突——输出张量 '{out_name}' 被多个节点重复使用",
                        node.line, node.column, "命名冲突"
                    ))
                else:
                    output_names[out_name] = node

        # 从graph的output定义中检查
        out_def_names = set()
        for outp in graph.outputs:
            if outp.name:
                if outp.name in out_def_names:
                    self.errors.append(SemanticError(
                        f"输出定义名称冲突——'{outp.name}' 在output中重复定义",
                        outp.line, outp.column, "命名冲突"
                    ))
                out_def_names.add(outp.name)

        # 检查input定义名称冲突
        input_def_names = set()
        for inp in graph.inputs:
            if inp.name:
                if inp.name in input_def_names:
                    self.errors.append(SemanticError(
                        f"输入定义名称冲突——'{inp.name}' 在input中重复定义",
                        inp.line, inp.column, "命名冲突"
                    ))
                input_def_names.add(inp.name)

        # 检查initializer名称冲突
        init_names = set()
        for init in graph.initializers:
            if init.name:
                if init.name in init_names:
                    self.errors.append(SemanticError(
                        f"初始化器名称冲突——'{init.name}' 重复定义",
                        init.line, init.column, "命名冲突"
                    ))
                init_names.add(init.name)

    def _check_undefined_references(self):
        """检查未定义即使用：输入张量必须先定义才能被引用"""
        graph = self.model.graph

        # 收集所有已定义的输入张量名称
        defined_inputs = set()
        for inp in graph.inputs:
            if inp.name:
                defined_inputs.add(inp.name)

        # 收集所有已定义的初始化器名称
        defined_initializers = set()
        for init in graph.initializers:
            if init.name:
                defined_initializers.add(init.name)

        # 收集所有节点的输出张量名称（中间变量）
        defined_outputs = set()
        for node in graph.nodes:
            for out_name in node.outputs:
                defined_outputs.add(out_name)

        # 检查每个节点的输入引用
        all_defined = defined_inputs | defined_initializers | defined_outputs
        for node in graph.nodes:
            for inp_name in node.inputs:
                if inp_name not in all_defined:
                    # 判断是在input还是initializer中缺失
                    # 先尝试从已定义的输入和初始化器中查找
                    self.errors.append(SemanticError(
                        f"输入张量 '{inp_name}' 未定义即被引用——应先定义才能被引用",
                        node.line, node.column, "未定义引用"
                    ))

    def _check_tensor_types(self):
        """张量类型检查：同一操作符输入类型一致、输入输出类型匹配"""
        graph = self.model.graph

        # 建立张量名到类型的映射
        tensor_types = {}

        # 从input定义中收集
        for inp in graph.inputs:
            if inp.name and inp.type_info and inp.type_info.tensor_type:
                tensor_types[inp.name] = inp.type_info.tensor_type.elem_type

        # 从output定义中收集
        for outp in graph.outputs:
            if outp.name and outp.type_info and outp.type_info.tensor_type:
                tensor_types[outp.name] = outp.type_info.tensor_type.elem_type

        # 从initializer中收集
        for init in graph.initializers:
            if init.name and init.data_type:
                tensor_types[init.name] = init.data_type

        # 检查同一操作符的输入张量类型是否一致
        for node in graph.nodes:
            if len(node.inputs) > 1:
                input_types = []
                for inp_name in node.inputs:
                    if inp_name in tensor_types:
                        input_types.append((inp_name, tensor_types[inp_name]))

                if len(input_types) > 1:
                    first_type = input_types[0][1]
                    for inp_name, t in input_types[1:]:
                        if t != first_type:
                            self.errors.append(SemanticError(
                                f"同一操作符的输入张量类型必须一致——"
                                f"{node.op_type} 操作符的输入张量类型不一致"
                                f"（{input_types[0][0]}={first_type}, {inp_name}={t}）",
                                node.line, node.column, "类型不匹配"
                            ))
                            break

        # 检查模型整体输入输出类型是否匹配
        # 常见操作符的输入输出类型应对应关系
        for node in graph.nodes:
            op = node.op_type
            # 检查输出类型
            for out_name in node.outputs:
                if out_name in tensor_types:
                    out_type = tensor_types[out_name]
                    # 对于某些操作符，输出类型应等于输入类型
                    type_constrained_ops = {
                        "Add", "Sub", "Mul", "Div", "MatMul",
                        "Conv", "Relu", "Sigmod", "Tanh",
                        "Pad", "Reshape", "Transpose",
                    }
                    if op in type_constrained_ops and node.inputs:
                        # 取第一个输入的类型作为参考
                        first_input = node.inputs[0]
                        if first_input in tensor_types:
                            expected_type = tensor_types[first_input]
                            if out_type != expected_type:
                                self.errors.append(SemanticError(
                                    f"模型整体输入输出不匹配——{op} 操作的输入为"
                                    f"{expected_type.upper()}类型，输出{out_name}为"
                                    f"{out_type.upper()}类型",
                                    node.line, node.column, "类型不匹配"
                                ))

    def print_errors(self):
        """打印语义错误"""
        if self.errors:
            print("=" * 60)
            print(f"语义分析错误（共 {len(self.errors)} 个）")
            print("=" * 60)
            for i, err in enumerate(self.errors, 1):
                print(f"  {i}. {err}")
        else:
            print("=" * 60)
            print("语义分析通过，未发现语义错误。")
            print("=" * 60)
