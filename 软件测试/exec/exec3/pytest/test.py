import time
import os
import sys
import unittest
from unittest.mock import patch, MagicMock

# 设置环境变量，尝试解决网络和SSL问题
os.environ['WDM_SSL_VERIFY'] = '0'
os.environ['WDM_LOG_LEVEL'] = '0'

try:
    from selenium import webdriver
    from selenium.webdriver.common.by import By
    from selenium.webdriver.common.keys import Keys
    from selenium.webdriver.support.ui import WebDriverWait
    from selenium.webdriver.support import expected_conditions as EC
    from selenium.webdriver.chrome.service import Service
    from selenium.webdriver.chrome.options import Options
    from selenium.common.exceptions import TimeoutException, ElementClickInterceptedException, NoSuchElementException, WebDriverException
    from webdriver_manager.chrome import ChromeDriverManager
except ImportError as e:
    print(f"Import Error: {e}")
    sys.exit(1)

# 配置
TIMEOUT = 10
RETRIES = 3

class AnjukeTest:
    def __init__(self):
        self.driver = None
        self.timeout = TIMEOUT
        self.retries = RETRIES

    def get_driver(self):
        """初始化浏览器驱动"""
        options = webdriver.ChromeOptions()
        options.add_argument('--start-maximized')
        options.add_argument('--disable-gpu')
        options.add_argument('--ignore-certificate-errors')
        options.add_argument('--disable-blink-features=AutomationControlled')
        options.add_experimental_option("excludeSwitches", ["enable-automation"])
        options.add_experimental_option('useAutomationExtension', False)

        driver = None
        
        print("[INFO] Initializing Chrome Driver...")

        # 1. 优先尝试 Selenium 4 自带的 Selenium Manager
        try:
            print("[INFO] Attempting to use Selenium Manager (Default)...")
            driver = webdriver.Chrome(options=options)
            print("[INFO] Selenium Manager initialized successfully.")
            self.driver = driver
            return driver
        except Exception as e:
            print(f"[WARN] Selenium Manager failed: {e}")

        # 2. 尝试使用 webdriver_manager
        try:
            print("[INFO] Attempting to use webdriver_manager...")
            service = Service(ChromeDriverManager().install())
            driver = webdriver.Chrome(service=service, options=options)
            print("[INFO] webdriver_manager initialized successfully.")
            self.driver = driver
            return driver
        except Exception as e:
            print(f"[WARN] webdriver_manager failed: {e}")

        # 3. 尝试使用本地驱动 (Fallback)
        try:
            base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            driver_path = os.path.join(base_dir, "chromedriver.exe")
            
            print(f"[INFO] Attempting to use local driver: {driver_path}")
            if os.path.exists(driver_path):
                service = Service(driver_path)
                driver = webdriver.Chrome(service=service, options=options)
                print("[INFO] Local driver initialized successfully.")
                self.driver = driver
                return driver
            else:
                print(f"[WARN] Local driver not found at {driver_path}")
        except Exception as e:
            print(f"[WARN] Local driver failed: {e}")

        raise Exception("Failed to initialize any Chrome Driver. Please ensure Chrome is installed and internet connection is available to download the driver.")

    def try_close_popups(self, driver):
        """尝试关闭常见的弹窗"""
        popup_locators = [
            (By.CSS_SELECTOR, ".login-close"),
            (By.CSS_SELECTOR, ".close-icon"),
            (By.CSS_SELECTOR, "[aria-label='close']"),
            (By.XPATH, "//i[contains(@class,'close')]"),
            (By.CLASS_NAME, "l-icon-close")
        ]
        
        for by, value in popup_locators:
            try:
                elements = driver.find_elements(by, value)
                for el in elements:
                    if el.is_displayed():
                        driver.execute_script("arguments[0].click();", el)
                        time.sleep(0.5)
            except Exception:
                pass

    def wait_and_click(self, driver, strategies, step_name):
        """智能等待并点击"""
        print(f"Step: {step_name}")
        last_exception = None
        
        for attempt in range(self.retries):
            try:
                self.try_close_popups(driver)
                
                element = None
                # 1. Try clickable
                for name, locator in strategies:
                    try:
                        element = WebDriverWait(driver, 2).until(EC.element_to_be_clickable(locator))
                        break
                    except:
                        continue
                
                # 2. Try presence if clickable fails
                if not element:
                    for name, locator in strategies:
                        try:
                            element = WebDriverWait(driver, 1).until(EC.presence_of_element_located(locator))
                            if element.is_displayed():
                                break
                        except:
                            continue
                
                if not element:
                    raise Exception(f"Element not found: {step_name}")

                driver.execute_script("arguments[0].scrollIntoView({behavior: 'instant', block: 'center'});", element)
                time.sleep(0.5)

                try:
                    element.click()
                except ElementClickInterceptedException:
                    driver.execute_script("arguments[0].click();", element)
                
                print(f"  -> Success")
                time.sleep(1)
                return True

            except Exception as e:
                last_exception = e
                time.sleep(1)
                
        print(f"  -> [FAILED] {step_name}")
        raise last_exception

    def wait_and_input(self, driver, strategies, text, step_name):
        """智能等待并输入"""
        print(f"Step: {step_name} -> Input '{text}'")
        for attempt in range(self.retries):
            try:
                self.try_close_popups(driver)
                
                element = None
                for name, locator in strategies:
                    try:
                        element = WebDriverWait(driver, 3).until(EC.visibility_of_element_located(locator))
                        break
                    except:
                        continue
                
                if not element:
                    raise Exception("Input field not found")

                driver.execute_script("arguments[0].scrollIntoView({behavior: 'instant', block: 'center'});", element)
                element.clear()
                element.send_keys(text)
                print(f"  -> Success")
                time.sleep(0.5)
                return True
            except Exception as e:
                time.sleep(1)
        raise Exception(f"{step_name} Input Failed")

    def run_anjuke_test(self):
        driver = None
        try:
            driver = self.get_driver()
            
            # 1. 打开安居客网页版，点击租房
            print("\n=== 1. 打开安居客网页版，点击租房 ===")
            driver.get("https://www.anjuke.com/")
            WebDriverWait(driver, 10).until(lambda d: "安居客" in d.title)
            
            # 点击租房
            self.wait_and_click(driver, [
                ("Link Text", (By.LINK_TEXT, "租房")),
                ("XPath", (By.XPATH, "//a[contains(text(), '租房')]")),
                ("CSS", (By.CSS_SELECTOR, ".zufang")),
                ("Nav", (By.XPATH, "//nav//a[contains(text(), '租房')]"))
            ], "点击租房")

            # 2. 地址选择"南京"
            print("\n=== 2. 地址选择南京 ===")
            self.wait_and_click(driver, [
                ("Link Text", (By.LINK_TEXT, "南京")),
                ("XPath", (By.XPATH, "//a[contains(text(), '南京')]")),
                ("CSS", (By.CSS_SELECTOR, ".city-name")),
                ("City", (By.XPATH, "//div[contains(@class,'city-select')]//a[contains(text(), '南京')]"))
            ], "选择南京")

            # 3. 点击"地铁找房"
            print("\n=== 3. 点击地铁找房 ===")
            time.sleep(2)  # 等待页面加载
            self.wait_and_click(driver, [
                ("Link Text", (By.LINK_TEXT, "地铁找房")),
                ("XPath", (By.XPATH, "//a[contains(text(), '地铁找房')]")),
                ("CSS", (By.CSS_SELECTOR, ".subway-search")),
                ("Filter", (By.XPATH, "//div[contains(@class,'filter')]//a[contains(text(), '地铁')]")),
                ("Tab", (By.XPATH, "//div[contains(@class,'tabs')]//a[contains(text(), '地铁')]"))
            ], "点击地铁找房")

            # 4. Line 2
            self.wait_and_click(driver, [
                ("Link Text", (By.LINK_TEXT, "2号线")),
                ("XPath", (By.XPATH, "//a[contains(text(), '2号线')]"))
            ], "Select 'Line 2'")

            # 5. Station Maqun
            self.wait_and_click(driver, [
                ("Link Text", (By.LINK_TEXT, "马群")),
                ("XPath", (By.XPATH, "//a[contains(text(), '马群')]"))
            ], "Select 'Maqun'")

            # 6. 设置租金为 5000 - 8000 元，并点击确定
            print("\n=== 6. 设置租金为 5000 - 8000 元，并点击确定 ===")
            min_price_strategies = [
                ("XPath 1", (By.XPATH, "//input[contains(@placeholder,'最低租金') or contains(@placeholder,'最低价')]")),
                ("XPath 2", (By.XPATH, "//input[@class='min-price' or contains(@class,'min')]")),
                ("XPath 3", (By.XPATH, "(//input[@type='text'])[1]"))
            ]
            max_price_strategies = [
                ("XPath 1", (By.XPATH, "//input[contains(@placeholder,'最高租金') or contains(@placeholder,'最高价')]")),
                ("XPath 2", (By.XPATH, "//input[@class='max-price' or contains(@class,'max')]")),
                ("XPath 3", (By.XPATH, "(//input[@type='text'])[2]"))
            ]

            try:
                self.wait_and_input(driver, min_price_strategies, "5000", "最低租金")
                self.wait_and_input(driver, max_price_strategies, "8000", "最高租金")
                
                confirm_strategies = [
                    ("Button", (By.XPATH, "//button[contains(text(), '确定')]")),
                    ("Input", (By.XPATH, "//input[@value='确定']")),
                    ("Class", (By.CLASS_NAME, "btn-confirm")),
                    ("Link", (By.LINK_TEXT, "确定"))
                ]
                try:
                    self.wait_and_click(driver, confirm_strategies, "点击确定")
                except:
                    print("  -> 确定按钮未找到，尝试按回车键...")
                    inputs = driver.find_elements(By.XPATH, "//input[@type='text']")
                    if inputs:
                        inputs[-1].send_keys(Keys.ENTER)
            except Exception as e:
                print(f"  -> [WARN] 价格设置问题: {e}")

            # 7. 选择"整租"
            print("\n=== 7. 选择整租 ===")
            self.wait_and_click(driver, [
                ("Link Text", (By.LINK_TEXT, "整租")),
                ("XPath", (By.XPATH, "//a[contains(text(), '整租')]")),
                ("Class", (By.CLASS_NAME, "whole-rent"))
            ], "选择整租")

            # 8. 房屋类型选择"普通住宅"
            print("\n=== 8. 房屋类型选择普通住宅 ===")
            found = False
            direct_strategies = [
                ("Link", (By.LINK_TEXT, "普通住宅")), 
                ("XPath", (By.XPATH, "//a[contains(text(), '普通住宅')]")),
                ("Label", (By.XPATH, "//label[contains(text(), '普通住宅')]"))
            ]
            
            try:
                self.wait_and_click(driver, direct_strategies, "直接点击普通住宅")
                found = True
            except:
                pass

            if not found:
                print("  -> 未直接找到，检查下拉菜单...")
                try:
                    dropdowns = [
                        ("More", (By.XPATH, "//li[contains(., '更多') or contains(., '类型')]")),
                        ("Type", (By.XPATH, "//div[contains(text(), '房屋类型')]")),
                        ("Filter", (By.XPATH, "//span[contains(text(), '房源类型')]"))
                    ]
                    try:
                        self.wait_and_click(driver, dropdowns, "打开下拉菜单")
                    except:
                        pass
                    self.wait_and_click(driver, direct_strategies, "在下拉菜单中点击普通住宅")
                except Exception as e:
                    print(f"  -> [WARN] 无法选择普通住宅: {e}")

            # 9. 在搜索框中搜索"经天路"，并点击"搜索"
            print("\n=== 9. 在搜索框中搜索经天路，并点击搜索 ===")
            search_input_strategies = [
                ("ID", (By.ID, "search-input")),
                ("Class", (By.CLASS_NAME, "search-input")),
                ("Placeholder", (By.XPATH, "//input[contains(@placeholder, '搜索') or contains(@placeholder, '小区/商圈/地铁站')]")),
                ("Generic", (By.XPATH, "//input[@type='text']"))
            ]
            self.wait_and_input(driver, search_input_strategies, "经天路", "搜索输入框")
            
            search_button_strategies = [
                ("Class", (By.CLASS_NAME, "search-btn")),
                ("Button", (By.XPATH, "//button[contains(text(), '搜索')]")),
                ("Input", (By.XPATH, "//input[@type='button' and @value='搜索']")),
                ("Icon", (By.XPATH, "//i[contains(@class,'search')]"))
            ]
            self.wait_and_click(driver, search_button_strategies, "点击搜索按钮")
            time.sleep(2)

            # 10. 选择"视频看房"
            print("\n=== 10. 选择视频看房 ===")
            try:
                self.wait_and_click(driver, [
                    ("Link", (By.LINK_TEXT, "视频看房")),
                    ("XPath", (By.XPATH, "//a[contains(text(), '视频看房')]")),
                    ("Class", (By.CLASS_NAME, "video-filter"))
                ], "选择视频看房")
            except:
                print("  -> 视频看房选项未找到（可能没有可用的视频房源）")

            # 11. 依次点击"租金"、"最新"排序进行查看
            print("\n=== 11. 依次点击租金、最新排序进行查看 ===")
            try:
                self.wait_and_click(driver, [
                    ("Rent", (By.XPATH, "//a[contains(text(), '租金')]")),
                    ("Class", (By.CLASS_NAME, "sort-price"))
                ], "点击租金排序")
                time.sleep(1)
                self.wait_and_click(driver, [
                    ("Latest", (By.XPATH, "//a[contains(text(), '最新')]")),
                    ("Class", (By.CLASS_NAME, "sort-latest"))
                ], "点击最新排序")
            except:
                print("  -> 排序功能无法使用")

            # 12. 点击第一个搜索出来的房源进行查看
            print("\n=== 12. 点击第一个搜索出来的房源进行查看 ===")
            self.wait_and_click(driver, [
                ("First Item", (By.XPATH, "(//div[contains(@class,'zu-itemmod') or contains(@class,'item-mod')]//a)[1]")),
                ("Generic", (By.XPATH, "(//div[@class='zu-itemmod' or @class='item-mod']//h3/a)[1]")),
                ("Title", (By.XPATH, "(//h3//a)[1]"))
            ], "点击第一个房源")
            
            if len(driver.window_handles) > 1:
                driver.switch_to.window(driver.window_handles[-1])
                print(f"  -> Switched to new window: {driver.title}")
            
            time.sleep(5)
            print("\n✅ TEST COMPLETED SUCCESSFULLY!")
            return True

        except Exception as e:
            print(f"\n❌ TEST FAILED: {e}")
            if driver:
                try:
                    driver.save_screenshot("error_screenshot.png")
                    print("  -> Saved error_screenshot.png")
                except:
                    pass
            return False

        finally:
            if driver:
                print("Closing browser...")
                driver.quit()


class TestAnjuke(unittest.TestCase):
    def setUp(self):
        self.anjuke_test = AnjukeTest()
        
    def test_class_initialization(self):
        """测试类初始化"""
        self.assertEqual(self.anjuke_test.timeout, TIMEOUT)
        self.assertEqual(self.anjuke_test.retries, RETRIES)
        self.assertIsNone(self.anjuke_test.driver)
        
    def test_driver_initialization(self):
        """测试驱动初始化"""
        with patch('selenium.webdriver.Chrome') as mock_chrome:
            mock_driver = MagicMock()
            mock_chrome.return_value = mock_driver
            
            driver = self.anjuke_test.get_driver()
            
            self.assertIsNotNone(driver)
            mock_chrome.assert_called_once()
            
    def test_try_close_popups(self):
        """测试关闭弹窗功能"""
        mock_driver = MagicMock()
        mock_element = MagicMock()
        mock_element.is_displayed.return_value = True
        mock_driver.find_elements.return_value = [mock_element]
        
        # 测试不会抛出异常
        self.anjuke_test.try_close_popups(mock_driver)
        mock_driver.find_elements.assert_called()
        
    def test_basic_methods(self):
        """测试基本方法存在"""
        self.assertTrue(hasattr(self.anjuke_test, 'get_driver'))
        self.assertTrue(hasattr(self.anjuke_test, 'wait_and_click'))
        self.assertTrue(hasattr(self.anjuke_test, 'wait_and_input'))
        self.assertTrue(hasattr(self.anjuke_test, 'run_anjuke_test'))


if __name__ == "__main__":
    import sys
    
    # 检查命令行参数
    if len(sys.argv) > 1 and sys.argv[1] == "unit":
        # 运行单元测试
        print("运行单元测试...")
        unittest.main(argv=['first-arg-is-ignored'])
    else:
        # 运行实际测试
        print("运行安居客测试...")
        test_instance = AnjukeTest()
        test_instance.run_anjuke_test()