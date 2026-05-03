"""
S-ONNXCompiler - AST节点定义
定义S-ONNX语言抽象语法树的所有节点类型
"""


class ASTNode:
    """AST节点基类"""
    def __init__(self, node_type: str, line: int = 0, column: int = 0):
        self.node_type = node_type
        self.line = line
        self.column = column
        self.children = []

    def add_child(self, child):
        if child is not None:
            self.children.append(child)
        return self

    def __repr__(self):
        return f"ASTNode({self.node_type})"

    def to_string(self, indent=0):
        """将AST树格式化为缩进字符串"""
        prefix = "  " * indent
        result = f"{prefix}{self._node_label()}\n"
        for child in self.children:
            result += child.to_string(indent + 1)
        return result

    def _node_label(self):
        """节点标签，子类可覆盖"""
        return self.node_type

    def print_tree(self):
        """打印格式化AST树"""
        print(self.to_string())


class ModelProtoNode(ASTNode):
    """模型根节点"""
    def __init__(self, line=0, column=0):
        super().__init__("ModelProto", line, column)
        self.ir_version = None
        self.producer_name = None
        self.producer_version = None
        self.domain = None
        self.model_version = None
        self.doc_string = None
        self.graph = None
        self.opset_import = None


class GraphNode(ASTNode):
    """计算图节点"""
    def __init__(self, line=0, column=0):
        super().__init__("Graph", line, column)
        self.name = None
        self.nodes = []       # 节点列表
        self.inputs = []      # 输入列表
        self.outputs = []     # 输出列表
        self.initializers = []  # 初始化器列表（可选）


class NodeDefNode(ASTNode):
    """计算节点定义"""
    def __init__(self, line=0, column=0):
        super().__init__("NodeDef", line, column)
        self.op_type = None   # 操作类型
        self.name = None      # 节点名称
        self.inputs = []      # 输入列表（input_arr形式）
        self.input_infos = [] # 输入信息列表（input_list形式）
        self.outputs = []     # 输出列表（output_arr形式）
        self.output_infos = []# 输出信息列表（output_list形式）
        self.attributes = []  # 属性列表（可选）


class InputInfoNode(ASTNode):
    """输入张量信息"""
    def __init__(self, line=0, column=0):
        super().__init__("InputInfo", line, column)
        self.name = None
        self.type_info = None


class OutputInfoNode(ASTNode):
    """输出张量信息"""
    def __init__(self, line=0, column=0):
        super().__init__("OutputInfo", line, column)
        self.name = None
        self.type_info = None


class TypeInfoNode(ASTNode):
    """类型信息"""
    def __init__(self, line=0, column=0):
        super().__init__("Type", line, column)
        self.tensor_type = None


class TensorTypeNode(ASTNode):
    """张量类型"""
    def __init__(self, line=0, column=0):
        super().__init__("TensorType", line, column)
        self.elem_type = None
        self.shape = None


class ShapeNode(ASTNode):
    """张量形状"""
    def __init__(self, line=0, column=0):
        super().__init__("Shape", line, column)
        self.dims = []


class DimNode(ASTNode):
    """维度节点"""
    def __init__(self, line=0, column=0):
        super().__init__("Dim", line, column)
        self.dim_value = None   # 整数维度值
        self.dim_param = None   # 动态维度参数


class AttributeNode(ASTNode):
    """属性节点"""
    def __init__(self, line=0, column=0):
        super().__init__("Attribute", line, column)
        self.name = None
        self.value = None


class InitializerNode(ASTNode):
    """初始化器节点"""
    def __init__(self, line=0, column=0):
        super().__init__("Initializer", line, column)
        self.name = None
        self.data_type = None
        self.dims = []
        self.raw_data = None


class OpsetImportNode(ASTNode):
    """算子集导入节点"""
    def __init__(self, line=0, column=0):
        super().__init__("OpsetImport", line, column)
        self.domain = None
        self.version = None


class NameNode(ASTNode):
    """名称节点"""
    def __init__(self, value: str, line=0, column=0):
        super().__init__("Name", line, column)
        self.value = value

    def _node_label(self):
        return f"Name: {self.value}"


class StringLiteralNode(ASTNode):
    """字符串字面量节点"""
    def __init__(self, value: str, line=0, column=0):
        super().__init__("StringLiteral", line, column)
        self.value = value

    def _node_label(self):
        return f'StringLiteral: "{self.value}"'


class IntegerNode(ASTNode):
    """整数字面量节点"""
    def __init__(self, value: str, line=0, column=0):
        super().__init__("Integer", line, column)
        self.value = value

    def _node_label(self):
        return f"Integer: {self.value}"


class BytesNode(ASTNode):
    """字节数据节点"""
    def __init__(self, value: str, line=0, column=0):
        super().__init__("Bytes", line, column)
        self.value = value

    def _node_label(self):
        return f"Bytes: {self.value}"


class DataTypeNode(ASTNode):
    """数据类型节点 (int/float/string/bool)"""
    def __init__(self, value: str, line=0, column=0):
        super().__init__("DataType", line, column)
        self.value = value

    def _node_label(self):
        return f"DataType: {self.value}"


# ========== 格式化打印AST ==========
def format_ast(node: ASTNode, indent=0) -> str:
    """格式化输出AST树结构"""
    if node is None:
        return "  " * indent + "None\n"

    prefix = "  " * indent
    lines = []

    if isinstance(node, ModelProtoNode):
        lines.append(f"{prefix}ModelProto")
        lines.append(f"{prefix}  ir_version: {node.ir_version}")
        lines.append(f"{prefix}  producer_name: {node.producer_name}")
        lines.append(f"{prefix}  producer_version: {node.producer_version}")
        lines.append(f"{prefix}  domain: {node.domain}")
        lines.append(f"{prefix}  model_version: {node.model_version}")
        lines.append(f"{prefix}  doc_string: {node.doc_string}")
        if node.graph:
            lines.append(format_ast(node.graph, indent + 1))
        if node.opset_import:
            lines.append(format_ast(node.opset_import, indent + 1))
    elif isinstance(node, GraphNode):
        lines.append(f"{prefix}Graph")
        lines.append(f"{prefix}  name: {node.name}")
        for n in node.nodes:
            lines.append(format_ast(n, indent + 1))
        for inp in node.inputs:
            lines.append(format_ast(inp, indent + 1))
        for outp in node.outputs:
            lines.append(format_ast(outp, indent + 1))
        for init in node.initializers:
            lines.append(format_ast(init, indent + 1))
    elif isinstance(node, NodeDefNode):
        lines.append(f"{prefix}Node")
        lines.append(f"{prefix}  op_type: {node.op_type}")
        lines.append(f"{prefix}  name: {node.name}")
        if node.inputs:
            lines.append(f"{prefix}  inputs: {node.inputs}")
        if node.input_infos:
            for inp in node.input_infos:
                lines.append(format_ast(inp, indent + 2))
        if node.outputs:
            lines.append(f"{prefix}  outputs: {node.outputs}")
        if node.output_infos:
            for outp in node.output_infos:
                lines.append(format_ast(outp, indent + 2))
        for attr in node.attributes:
            lines.append(format_ast(attr, indent + 1))
    elif isinstance(node, InputInfoNode):
        lines.append(f"{prefix}Input")
        lines.append(f"{prefix}  name: {node.name}")
        if node.type_info:
            lines.append(format_ast(node.type_info, indent + 1))
    elif isinstance(node, OutputInfoNode):
        lines.append(f"{prefix}Output")
        lines.append(f"{prefix}  name: {node.name}")
        if node.type_info:
            lines.append(format_ast(node.type_info, indent + 1))
    elif isinstance(node, TypeInfoNode):
        lines.append(f"{prefix}Type")
        if node.tensor_type:
            lines.append(format_ast(node.tensor_type, indent + 1))
    elif isinstance(node, TensorTypeNode):
        lines.append(f"{prefix}TensorType")
        lines.append(f"{prefix}  elem_type: {node.elem_type}")
        if node.shape:
            lines.append(format_ast(node.shape, indent + 1))
    elif isinstance(node, ShapeNode):
        lines.append(f"{prefix}Shape")
        for d in node.dims:
            lines.append(format_ast(d, indent + 1))
    elif isinstance(node, DimNode):
        if node.dim_value is not None:
            lines.append(f"{prefix}Dim(dim_value={node.dim_value})")
        else:
            lines.append(f"{prefix}Dim(dim_param={node.dim_param})")
    elif isinstance(node, AttributeNode):
        lines.append(f"{prefix}Attribute")
        lines.append(f"{prefix}  name: {node.name}")
        lines.append(f"{prefix}  value: {node.value}")
    elif isinstance(node, InitializerNode):
        lines.append(f"{prefix}Initializer")
        lines.append(f"{prefix}  name: {node.name}")
        lines.append(f"{prefix}  data_type: {node.data_type}")
        lines.append(f"{prefix}  dims: {node.dims}")
        lines.append(f"{prefix}  raw_data: {node.raw_data}")
    elif isinstance(node, OpsetImportNode):
        lines.append(f"{prefix}OpsetImport")
        lines.append(f"{prefix}  domain: {node.domain}")
        lines.append(f"{prefix}  version: {node.version}")
    else:
        lines.append(f"{prefix}{node._node_label()}")

    return "\n".join(lines)
