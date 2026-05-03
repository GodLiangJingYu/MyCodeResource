"""
S-ONNXCompiler - CodeGenerator (中间代码生成器)
将S-ONNX模型转换为三地址码(TAC)表示的中间代码
后序遍历AST生成中间代码
"""

from ast_nodes import *


class ThreeAddressCode:
    """三地址码指令"""

    def __init__(self, result=None, operation=None, operands=None, attributes=None):
        self.result = result        # 输出变量名
        self.operation = operation   # 操作类型
        self.operands = operands or []    # 操作数列表
        self.attributes = attributes or {}  # 属性字典

    def __str__(self):
        if self.operation == "Input":
            # <result> = Input(<name>, <data_type>, <shape>)
            shape_str = "[" + ", ".join(self.attributes.get("shape", [])) + "]"
            dtype = self.attributes.get("data_type", "UNKNOWN").upper()
            return f'{self.result} = Input("{self.attributes.get("name", "")}", {dtype}, {shape_str})'

        elif self.operation == "Output":
            # Output(<name>, <operand>)
            return f'Output("{self.attributes.get("name", "")}", {self.operands[0] if self.operands else ""})'

        elif self.operation == "Initializer":
            # <result> = Initializer(<name>, <data_type>, <shape>, raw_data=...)
            shape_str = "[" + ", ".join(self.attributes.get("shape", [])) + "]"
            dtype = self.attributes.get("data_type", "UNKNOWN").upper()
            raw = self.attributes.get("raw_data", "")
            return f'{self.result} = Initializer("{self.attributes.get("name", "")}", {dtype}, {shape_str}, raw_data={raw})'

        elif self.operation in ("Conv", "Add", "Sub", "Mul", "Div", "MatMul",
                                "Relu", "Pad", "Reshape", "CustomOp",
                                "Sigmod", "Tanh", "Softmax", "Transpose"):
            # <result> = <op_type>(<operand1>, <operand2>, ..., <attributes>)
            ops = ", ".join(self.operands)
            attr_str = ""
            if self.attributes:
                attr_parts = []
                for k, v in self.attributes.items():
                    attr_parts.append(f'{k}="{v}"')
                attr_str = ", " + ", ".join(attr_parts) if attr_parts else ""
            return f'{self.result} = {self.operation}({ops}{attr_str})'

        else:
            # 通用格式
            ops = ", ".join(self.operands)
            return f'{self.result} = {self.operation}({ops})'

    def __repr__(self):
        return str(self)


class CodeGenerator:
    """
    S-ONNX中间代码生成器
    后序遍历AST，生成三地址码
    """

    def __init__(self, model: ModelProtoNode):
        self.model = model
        self.instructions = []  # 三地址码指令列表
        self.temp_counter = 0   # 临时变量计数器
        self.var_map = {}       # 张量名到临时变量的映射

    def _new_temp(self) -> str:
        """生成新的临时变量名"""
        self.temp_counter += 1
        return f"T{self.temp_counter}"

    def generate(self) -> list:
        """生成中间代码（后序遍历AST）"""
        self.instructions = []
        self.temp_counter = 0
        self.var_map = {}

        if self.model.graph is None:
            return self.instructions

        graph = self.model.graph

        # 1. 处理输入张量定义
        for inp in graph.inputs:
            self._gen_input(inp)

        # 2. 处理初始化器
        for init in graph.initializers:
            self._gen_initializer(init)

        # 3. 处理计算节点（按顺序）
        for node in graph.nodes:
            self._gen_node(node)

        # 4. 处理输出张量定义
        for outp in graph.outputs:
            self._gen_output(outp)

        return self.instructions

    def _gen_input(self, inp: InputInfoNode):
        """生成输入张量定义的三地址码"""
        temp = self._new_temp()
        name = inp.name
        self.var_map[name] = temp

        data_type = "UNKNOWN"
        shape = []
        if inp.type_info and inp.type_info.tensor_type:
            data_type = inp.type_info.tensor_type.elem_type or "UNKNOWN"
            if inp.type_info.tensor_type.shape:
                shape = []
                for d in inp.type_info.tensor_type.shape.dims:
                    if d.dim_value is not None:
                        shape.append(str(d.dim_value))
                    elif d.dim_param is not None:
                        shape.append(d.dim_param)

        tac = ThreeAddressCode(
            result=temp,
            operation="Input",
            operands=[],
            attributes={
                "name": name,
                "data_type": data_type,
                "shape": shape,
            }
        )
        self.instructions.append(tac)

    def _gen_initializer(self, init: InitializerNode):
        """生成权重初始化的三地址码"""
        temp = self._new_temp()
        name = init.name
        self.var_map[name] = temp

        shape = [str(d) for d in init.dims] if init.dims else []

        tac = ThreeAddressCode(
            result=temp,
            operation="Initializer",
            operands=[],
            attributes={
                "name": name,
                "data_type": init.data_type or "UNKNOWN",
                "shape": shape,
                "raw_data": init.raw_data or "",
            }
        )
        self.instructions.append(tac)

    def _gen_node(self, node: NodeDefNode):
        """生成计算操作的三地址码"""
        # 为每个输出分配临时变量
        output_temps = []
        for out_name in node.outputs:
            temp = self._new_temp()
            self.var_map[out_name] = temp
            output_temps.append(temp)

        # 如果只有一个输出，直接作为result
        result = output_temps[0] if output_temps else self._new_temp()

        # 收集输入操作数对应的临时变量
        operands = []
        for inp_name in node.inputs:
            if inp_name in self.var_map:
                operands.append(self.var_map[inp_name])
            else:
                operands.append(inp_name)  # 未找到映射，使用原名

        # 收集属性
        attributes = {}
        for attr in node.attributes:
            attributes[attr.name] = attr.value

        # 生成三地址码
        tac = ThreeAddressCode(
            result=result,
            operation=node.op_type,
            operands=operands,
            attributes=attributes,
        )
        self.instructions.append(tac)

        # 如果有多个输出，为其余输出生成赋值
        for i, (out_name, temp) in enumerate(zip(node.outputs, output_temps)):
            if i > 0:
                self.var_map[out_name] = temp

    def _gen_output(self, outp: OutputInfoNode):
        """生成输出张量定义的三地址码"""
        name = outp.name
        operand = self.var_map.get(name, name)

        tac = ThreeAddressCode(
            operation="Output",
            operands=[operand],
            attributes={"name": name},
        )
        self.instructions.append(tac)

    def print_code(self):
        """打印生成的三地址码"""
        print("=" * 60)
        print("中间代码生成结果（三地址码）")
        print("=" * 60)
        for i, tac in enumerate(self.instructions, 1):
            print(f"  {i:3d}: {tac}")
        print("=" * 60)
        print(f"共生成 {len(self.instructions)} 条三地址码指令")

    def get_code_string(self) -> str:
        """返回三地址码的字符串表示"""
        lines = []
        for i, tac in enumerate(self.instructions, 1):
            lines.append(f"  {i:3d}: {tac}")
        return "\n".join(lines)
