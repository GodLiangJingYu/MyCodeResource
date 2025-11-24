import re

with open(r"D:\MyCodeResource\科研方法\temp_content.txt", "r", encoding="utf-8") as f:
    text = f.read()

# Count words using regex to split on whitespace
words = re.split(r'\s+', text.strip())
word_count = len(words) if words and words[0] else 0

print(f"Word count: {word_count}")