"""
S-ONNXCompiler - ErrorHandler (错误处理模块)
统一管理词法错误、语法错误和语义错误
提供错误报告机制，包括错误定位和错误描述
"""

from lexer import LexerError
from parser import ParseError
from semantic_analyzer import SemanticError


class CompilerError:
    """编译器统一错误表示"""

    # 错误分类
    LEXICAL = "词法错误"
    SYNTACTIC = "语法错误"
    SEMANTIC = "语义错误"

    def __init__(self, error_type: str, message: str, line: int = 0, column: int = 0):
        self.error_type = error_type  # 错误类型
        self.message = message        # 错误描述
        self.line = line              # 行号
        self.column = column          # 列号

    def __str__(self):
        return f"[{self.error_type}] 第{self.line}行 第{self.column}列: {self.message}"

    def __repr__(self):
        return str(self)


class ErrorHandler:
    """
    S-ONNX编译器错误处理器
    统一收集、分类和报告所有编译错误
    """

    def __init__(self):
        self.errors = []

    def add_lexer_errors(self, lexer_errors: list):
        """添加词法错误"""
        for err in lexer_errors:
            self.errors.append(CompilerError(
                CompilerError.LEXICAL,
                err.message if hasattr(err, 'message') else str(err),
                err.line if hasattr(err, 'line') else 0,
                err.column if hasattr(err, 'column') else 0,
            ))

    def add_parser_errors(self, parser_errors: list):
        """添加语法错误"""
        for err in parser_errors:
            self.errors.append(CompilerError(
                CompilerError.SYNTACTIC,
                err.message if hasattr(err, 'message') else str(err),
                err.line if hasattr(err, 'line') else 0,
                err.column if hasattr(err, 'column') else 0,
            ))

    def add_semantic_errors(self, semantic_errors: list):
        """添加语义错误"""
        for err in semantic_errors:
            self.errors.append(CompilerError(
                CompilerError.SEMANTIC,
                err.message if hasattr(err, 'message') else str(err),
                err.line if hasattr(err, 'line') else 0,
                err.column if hasattr(err, 'column') else 0,
            ))

    def has_errors(self) -> bool:
        """是否存在错误"""
        return len(self.errors) > 0

    def get_errors_by_type(self, error_type: str) -> list:
        """按类型获取错误"""
        return [e for e in self.errors if e.error_type == error_type]

    def get_all_errors(self) -> list:
        """获取所有错误"""
        return self.errors

    def print_report(self):
        """打印错误报告"""
        print("=" * 70)
        print("S-ONNXCompiler 错误报告")
        print("=" * 70)

        if not self.errors:
            print("编译通过，未发现任何错误。")
            return

        # 按类型分组显示
        for error_type in [CompilerError.LEXICAL, CompilerError.SYNTACTIC, CompilerError.SEMANTIC]:
            type_errors = self.get_errors_by_type(error_type)
            if type_errors:
                print(f"\n--- {error_type} ({len(type_errors)} 个) ---")
                for i, err in enumerate(type_errors, 1):
                    print(f"  {i}. {err}")

        print(f"\n共发现 {len(self.errors)} 个错误")
        print("=" * 70)

    def get_report_string(self) -> str:
        """返回错误报告字符串"""
        if not self.errors:
            return "编译通过，未发现任何错误。"

        lines = []
        for error_type in [CompilerError.LEXICAL, CompilerError.SYNTACTIC, CompilerError.SEMANTIC]:
            type_errors = self.get_errors_by_type(error_type)
            if type_errors:
                lines.append(f"--- {error_type} ({len(type_errors)} 个) ---")
                for i, err in enumerate(type_errors, 1):
                    lines.append(f"  {i}. {err}")

        lines.append(f"共发现 {len(self.errors)} 个错误")
        return "\n".join(lines)
