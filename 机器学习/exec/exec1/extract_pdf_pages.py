import PyPDF2
import os

def extract_pages(input_pdf, output_pdf1, output_pdf2, pages1, pages2):
    """提取PDF的特定页面到新文件"""
    
    # 打开原始PDF文件
    with open(input_pdf, 'rb') as file:
        reader = PyPDF2.PdfReader(file)
        
        # 创建第一个输出PDF (页面41-44)
        writer1 = PyPDF2.PdfWriter()
        for page_num in pages1:
            if page_num <= len(reader.pages):
                writer1.add_page(reader.pages[page_num - 1])  # PDF页面从0开始计数
        
        with open(output_pdf1, 'wb') as output_file:
            writer1.write(output_file)
        
        # 创建第二个输出PDF (页面45-50)
        writer2 = PyPDF2.PdfWriter()
        for page_num in pages2:
            if page_num <= len(reader.pages):
                writer2.add_page(reader.pages[page_num - 1])
        
        with open(output_pdf2, 'wb') as output_file:
            writer2.write(output_file)

if __name__ == "__main__":
    input_file = "1机器学习开发编程基本运算熟悉(MATLAB为例).pdf"
    output_file1 = "pages_41-44.pdf"
    output_file2 = "pages_45-50.pdf"
    
    # 页面范围
    pages_range1 = list(range(41, 45))  # 41-44页
    pages_range2 = list(range(45, 51))  # 45-50页
    
    try:
        extract_pages(input_file, output_file1, output_file2, pages_range1, pages_range2)
        print(f"成功提取页面到 {output_file1} 和 {output_file2}")
    except Exception as e:
        print(f"提取页面时出错: {e}")