"""
S-ONNXCompiler - Lexer (词法分析器)
实现S-ONNX语言的词法分析，将源代码切分为词法单元(Token)
"""

import re
from enum import Enum, auto


class TokenType(Enum):
    """词法单元类型"""
    # 关键字 (32个)
    KW_MODELPROTO = "ModelProto"
    KW_GRAPH = "graph"
    KW_NAME = "name"
    KW_NODE = "node"
    KW_INPUT = "input"
    KW_OUTPUT = "output"
    KW_OP_TYPE = "op_type"
    KW_ATTRIBUTE = "attribute"
    KW_INITIALIZER = "initializer"
    KW_DOC_STRING = "doc_string"
    KW_DOMAIN = "domain"
    KW_MODEL_VERSION = "model_version"
    KW_PRODUCER_NAME = "producer_name"
    KW_PRODUCER_VERSION = "producer_version"
    KW_TYPE = "type"
    KW_TENSOR_TYPE = "tensor_type"
    KW_IR_VERSION = "ir_version"
    KW_ELEM_TYPE = "elem_type"
    KW_SHAPE = "shape"
    KW_DIM = "dim"
    KW_DIMS = "dims"
    KW_RAW_DATA = "raw_data"
    KW_OPSET_IMPORT = "opset_import"
    KW_DIM_VALUE = "dim_value"
    KW_DIM_PARAM = "dim_param"
    KW_DATA_TYPE = "data_type"
    KW_VERSION = "version"
    KW_VALUE = "value"
    KW_INT = "int"
    KW_FLOAT = "float"
    KW_STRING = "string"
    KW_BOOL = "bool"

    # 专用符号
    LBRACKET = "["
    RBRACKET = "]"
    LBRACE = "{"
    RBRACE = "}"
    COMMA = ","
    ASSIGN = "="

    # 字面量类型
    INTEGER = "INTEGER"
    STRING_LIT = "STRING"
    BYTES = "BYTES"
    IDENTIFIER = "IDENTIFIER"

    # 特殊
    EOF = "EOF"
    ERROR = "ERROR"


# 关键字映射表（不区分大小写）
KEYWORDS = {
    "modelproto": TokenType.KW_MODELPROTO,
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
}


class Token:
    """词法单元"""
    def __init__(self, token_type: TokenType, value: str, line: int, column: int):
        self.token_type = token_type
        self.value = value
        self.line = line
        self.column = column

    def __repr__(self):
        return f"Token({self.token_type.name}, {repr(self.value)}, line={self.line}, col={self.column})"

    def __str__(self):
        return f"<{self.token_type.name}, {repr(self.value)}>"


class LexerError(Exception):
    """词法错误"""
    def __init__(self, message: str, line: int, column: int, snippet: str = ""):
        self.message = message
        self.line = line
        self.column = column
        self.snippet = snippet
        super().__init__(self.format_error())

    def format_error(self):
        msg = f"[词法错误] 第{self.line}行 第{self.column}列: {self.message}"
        if self.snippet:
            msg += f"\n  代码片段: {self.snippet}"
        return msg


class Lexer:
    """
    S-ONNX词法分析器
    基于DFA(确定有穷自动机)实现
    """

    def __init__(self, source: str, filename: str = "<stdin>"):
        self.source = source
        self.filename = filename
        self.pos = 0        # 当前字符位置
        self.line = 1       # 当前行号
        self.column = 1     # 当前列号
        self.tokens = []    # 已识别的词法单元列表
        self.errors = []    # 词法错误列表
        self._tokenize()

    def _current_char(self):
        """获取当前字符"""
        if self.pos < len(self.source):
            return self.source[self.pos]
        return None

    def _peek_char(self, offset=1):
        """向前看offset个字符"""
        idx = self.pos + offset
        if idx < len(self.source):
            return self.source[idx]
        return None

    def _advance(self):
        """移动到下一个字符"""
        ch = self.source[self.pos]
        self.pos += 1
        if ch == '\n':
            self.line += 1
            self.column = 1
        else:
            self.column += 1
        return ch

    def _get_line_snippet(self):
        """获取当前行的代码片段"""
        start = self.source.rfind('\n', 0, self.pos) + 1
        end = self.source.find('\n', self.pos)
        if end == -1:
            end = len(self.source)
        return self.source[start:end].strip()

    def _skip_whitespace(self):
        """跳过空白字符（空格、制表符、换行）"""
        while self.pos < len(self.source):
            ch = self._current_char()
            if ch in (' ', '\t', '\r', '\n'):
                self._advance()
            else:
                break

    def _read_integer(self) -> Token:
        """读取整数字面量: (0 | [1-9][0-9]*) (l|L)?"""
        start_line = self.line
        start_col = self.column
        buf = ""

        ch = self._current_char()
        if ch == '0':
            buf += self._advance()
            # 检查是否为BYTES（以16进制开头后跟b）
            # 先不判断，INTEGER后续可能跟着b成为BYTES
        else:
            # [1-9][0-9]*
            buf += self._advance()
            while self.pos < len(self.source) and self._current_char().isdigit():
                buf += self._advance()

        # 检查是否有类型后缀 l|L
        if self.pos < len(self.source) and self._current_char() in ('l', 'L'):
            buf += self._advance()

        return Token(TokenType.INTEGER, buf, start_line, start_col)

    def _read_string(self) -> Token:
        """读取字符串字面量: "(...)"  """
        start_line = self.line
        start_col = self.column
        buf = '"'
        self._advance()  # 跳过开头的 "

        while self.pos < len(self.source):
            ch = self._current_char()
            if ch == '\\':
                # 转义序列
                buf += self._advance()  # 添加 \
                if self.pos < len(self.source):
                    next_ch = self._current_char()
                    if next_ch in ('b', 't', 'n', 'f', 'r', '"', "'", '\\'):
                        buf += self._advance()
                    else:
                        # 非法转义字符，报错但继续
                        err_ch = self._advance()
                        buf += err_ch
                        err = LexerError(
                            f"非法转义序列 '\\{err_ch}'",
                            self.line, self.column,
                            self._get_line_snippet()
                        )
                        self.errors.append(err)
                else:
                    err = LexerError(
                        "字符串在转义字符后意外结束",
                        self.line, self.column,
                        self._get_line_snippet()
                    )
                    self.errors.append(err)
                    break
            elif ch == '"':
                buf += self._advance()  # 添加结尾的 "
                return Token(TokenType.STRING_LIT, buf, start_line, start_col)
            elif ch == '\n':
                # 字符串中出现未转义的换行
                err = LexerError(
                    "字符串字面量中含有未转义的换行符",
                    self.line, self.column,
                    self._get_line_snippet()
                )
                self.errors.append(err)
                buf += self._advance()
            else:
                buf += self._advance()

        # 字符串未闭合
        err = LexerError(
            "字符串字面量未闭合（缺少结尾的双引号）",
            start_line, start_col,
            self._get_line_snippet()
        )
        self.errors.append(err)
        return Token(TokenType.STRING_LIT, buf, start_line, start_col)

    def _read_identifier_or_keyword(self) -> Token:
        """
        读取标识符或关键字
        标识符/关键字以字母或下划线开头，后跟字母、数字或下划线
        """
        start_line = self.line
        start_col = self.column
        buf = ""

        while self.pos < len(self.source):
            ch = self._current_char()
            if ch.isalpha() or ch.isdigit() or ch == '_':
                buf += self._advance()
            else:
                break

        # 关键字不区分大小写，转换为小写查找
        lower_buf = buf.lower()
        if lower_buf in KEYWORDS:
            tok_type = KEYWORDS[lower_buf]
            return Token(tok_type, buf, start_line, start_col)
        else:
            return Token(TokenType.IDENTIFIER, buf, start_line, start_col)

    def _read_hex_bytes(self, prefix: str, start_line: int, start_col: int) -> Token:
        """
        读取BYTES字面量: [0-9A-Fa-f]+b
        前缀已经读取，继续读取剩余的16进制字符和结尾的 b
        """
        buf = prefix
        while self.pos < len(self.source):
            ch = self._current_char()
            if ch in '0123456789ABCDEFabcdef':
                buf += self._advance()
            elif ch == 'b' or ch == 'B':
                # 结尾的 b（字节数据标志）
                buf += self._advance()
                return Token(TokenType.BYTES, buf, start_line, start_col)
            else:
                break

        # 没有以 b 结尾，但已经读取了十六进制字符，当作标识符返回
        return Token(TokenType.IDENTIFIER, buf, start_line, start_col)

    def _tokenize(self):
        """主词法分析循环"""
        while self.pos < len(self.source):
            self._skip_whitespace()
            if self.pos >= len(self.source):
                break

            ch = self._current_char()
            start_line = self.line
            start_col = self.column

            # 单字符专用符号
            if ch == '[':
                self._advance()
                self.tokens.append(Token(TokenType.LBRACKET, '[', start_line, start_col))
            elif ch == ']':
                self._advance()
                self.tokens.append(Token(TokenType.RBRACKET, ']', start_line, start_col))
            elif ch == '{':
                self._advance()
                self.tokens.append(Token(TokenType.LBRACE, '{', start_line, start_col))
            elif ch == '}':
                self._advance()
                self.tokens.append(Token(TokenType.RBRACE, '}', start_line, start_col))
            elif ch == ',':
                self._advance()
                self.tokens.append(Token(TokenType.COMMA, ',', start_line, start_col))
            elif ch == '=':
                self._advance()
                self.tokens.append(Token(TokenType.ASSIGN, '=', start_line, start_col))
            elif ch == '"':
                # 字符串字面量
                tok = self._read_string()
                self.tokens.append(tok)
            elif ch.isdigit():
                # 可能是INTEGER或BYTES
                # BYTES: [0-9A-Fa-f]+b  （b是终止标志，不是hex内容）
                # 关键区分：b后面如果不再是hex字符，则b是BYTES终止符
                #          b后面还是hex字符，则b是hex内容的一部分继续收集

                # 收集可能的十六进制字符
                hex_chars = ""
                while self.pos < len(self.source) and (
                    self._current_char() in '0123456789ABCDEFabcdef'
                ):
                    next_ch = self._current_char()
                    if next_ch in ('b', 'B'):
                        # 检查b后面是否还有hex字符
                        after_b = self._peek_char(1)
                        if after_b is None or after_b not in '0123456789ABCDEFabcdef':
                            # b是BYTES终止符
                            break
                        # 否则b是hex内容的一部分，继续收集
                    hex_chars += self._advance()

                if self.pos < len(self.source) and self._current_char() in ('b', 'B'):
                    # 消费BYTES终止符b
                    b_char = self._advance()
                    self.tokens.append(Token(TokenType.BYTES, hex_chars + b_char, start_line, start_col))
                else:
                    # 这是INTEGER
                    # 验证是否为合法整数：(0 | [1-9][0-9]*)
                    # 从hex_chars中提取纯数字部分（去掉A-F等非十进制字符）
                    # 如果含有A-Fa-f，说明源码有歧义但按INTEGER处理
                    val = hex_chars
                    # 检查是否有类型后缀 l|L
                    if self.pos < len(self.source) and self._current_char() in ('l', 'L'):
                        val += self._advance()
                    self.tokens.append(Token(TokenType.INTEGER, val, start_line, start_col))

            elif ch.isalpha() or ch == '_':
                # 标识符或关键字
                tok = self._read_identifier_or_keyword()
                # 检查是否紧跟 b，形成BYTES
                self.tokens.append(tok)
            else:
                # 非法字符
                illegal_ch = self._advance()
                err = LexerError(
                    f"非法字符 '{illegal_ch}' (ASCII: {ord(illegal_ch)})",
                    start_line, start_col,
                    self._get_line_snippet()
                )
                self.errors.append(err)
                self.tokens.append(Token(TokenType.ERROR, illegal_ch, start_line, start_col))

        # 添加EOF标记
        self.tokens.append(Token(TokenType.EOF, "", self.line, self.column))

    def get_tokens(self):
        """返回所有词法单元列表"""
        return self.tokens

    def print_tokens(self):
        """打印所有词法单元"""
        print("=" * 60)
        print("词法分析结果（Token序列）")
        print("=" * 60)
        print(f"{'序号':<6} {'类型':<25} {'词值':<30} {'行':<6} {'列'}")
        print("-" * 75)
        for i, tok in enumerate(self.tokens):
            if tok.token_type == TokenType.EOF:
                break
            print(f"{i:<6} {tok.token_type.name:<25} {repr(tok.value):<30} {tok.line:<6} {tok.column}")
        print("-" * 75)
        print(f"共识别 {len(self.tokens) - 1} 个词法单元")
        if self.errors:
            print(f"\n词法错误 ({len(self.errors)} 个):")
            for err in self.errors:
                print(f"  {err}")
