"""
S-ONNXCompiler - 主程序入口
整合词法分析、语法分析、语义分析、代码生成和错误处理模块
"""

import sys
import os

# 添加src目录到路径
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from lexer import Lexer, TokenType
from parser import Parser
from ast_nodes import ModelProtoNode, format_ast
from semantic_analyzer import SemanticAnalyzer
from code_generator import CodeGenerator
from error_handler import ErrorHandler


class SOnnxCompiler:
    """S-ONNX编译器主类"""

    def __init__(self):
        self.lexer = None
        self.parser = None
        self.ast = None
        self.semantic_analyzer = None
        self.code_generator = None
        self.error_handler = ErrorHandler()

    def compile(self, source: str, filename: str = "<stdin>") -> bool:
        """
        编译S-ONNX源代码
        返回True表示编译成功（可能有警告），False表示有错误
        """
        print("=" * 70)
        print(f"S-ONNXCompiler - 编译 {filename}")
        print("=" * 70)

        # ===== 第一步：词法分析 =====
        print("\n>>> 第一步：词法分析")
        print("-" * 40)
        self.lexer = Lexer(source, filename)
        self.lexer.print_tokens()

        # 收集词法错误
        if self.lexer.errors:
            self.error_handler.add_lexer_errors(self.lexer.errors)

        # ===== 第二步：语法分析 =====
        print("\n>>> 第二步：语法分析")
        print("-" * 40)
        self.parser = Parser(self.lexer.get_tokens())
        self.ast = self.parser.parse()

        # 收集语法错误
        if self.parser.get_errors():
            self.error_handler.add_parser_errors(self.parser.get_errors())

        # 输出抽象语法树
        if self.ast:
            print("\n抽象语法树（AST）：")
            print(format_ast(self.ast))

        # ===== 第三步：语义分析 =====
        print("\n>>> 第三步：语义分析")
        print("-" * 40)
        if self.ast:
            self.semantic_analyzer = SemanticAnalyzer(self.ast)
            semantic_errors = self.semantic_analyzer.analyze()
            self.semantic_analyzer.print_errors()
            self.error_handler.add_semantic_errors(semantic_errors)

        # ===== 第四步：中间代码生成 =====
        print("\n>>> 第四步：中间代码生成")
        print("-" * 40)
        if self.ast and self.ast.graph:
            self.code_generator = CodeGenerator(self.ast)
            self.code_generator.generate()
            self.code_generator.print_code()

        # ===== 错误报告 =====
        print("\n>>> 编译总结")
        print("-" * 40)
        self.error_handler.print_report()

        return not self.error_handler.has_errors()

    def get_ast_string(self):
        """获取AST的字符串表示"""
        if self.ast:
            return format_ast(self.ast)
        return ""

    def get_tac_string(self):
        """获取三地址码的字符串表示"""
        if self.code_generator:
            return self.code_generator.get_code_string()
        return ""


def main():
    """主函数"""
    if len(sys.argv) < 2:
        print("未提供命令行参数，默认运行 tests 目录下的所有测试文件...")
        base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        test_dir = os.path.join(base_dir, 'tests')
        import glob
        files_to_run = sorted(glob.glob(os.path.join(test_dir, '*.txt')))
        if not files_to_run:
            print(f"在 {test_dir} 中未找到任何 .txt 测试文件")
            sys.exit(1)
    else:
        files_to_run = sys.argv[1:]

    for filepath in files_to_run:
        if not os.path.exists(filepath):
            print(f"错误: 文件 '{filepath}' 不存在")
            continue

        with open(filepath, 'r', encoding='utf-8') as f:
            source = f.read()

        compiler = SOnnxCompiler()
        success = compiler.compile(source, filepath)
        print("\n" + "#" * 70 + "\n")


if __name__ == "__main__":
    main()
