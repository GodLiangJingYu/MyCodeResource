const { Builder, By, Key, until } = require('selenium-webdriver');
const assert = require('assert');
const fs = require('fs');
const path = require('path');

// 测试配置
const TEST_CONFIG = {
    baseUrl: 'https://nj.zu.anjuke.com/',
    timeout: {
        default: 10000,
        long: 15000,
        short: 5000
    },
    retries: 3,
    screenshotDir: path.join(__dirname, 'screenshots')
};

// 测试结果收集器
class TestResultCollector {
    constructor() {
        this.results = {
            passed: 0,
            failed: 0,
            details: [],
            startTime: new Date(),
            endTime: null
        };
    }

    addResult(step, status, error = null) {
        const result = {
            step,
            status,
            timestamp: new Date().toISOString(),
            error: error ? error.message : null
        };

        this.results.details.push(result);

        if (status === 'PASS') {
            this.results.passed++;
        } else {
            this.results.failed++;
        }

        console.log(`${status === 'PASS' ? '✓' : '✗'} ${step}: ${status === 'PASS' ? '成功' : '失败'}`);
        if (error) {
            console.log(`  错误: ${error.message}`);
        }
    }

    finish() {
        this.results.endTime = new Date();
        this.generateReport();
    }

    generateReport() {
        const duration = this.results.endTime - this.results.startTime;
        const report = {
            summary: {
                total: this.results.passed + this.results.failed,
                passed: this.results.passed,
                failed: this.results.failed,
                duration: `${(duration / 1000).toFixed(2)}秒`,
                successRate: `${((this.results.passed / (this.results.passed + this.results.failed)) * 100).toFixed(2)}%`
            },
            details: this.results.details,
            startTime: this.results.startTime.toISOString(),
            endTime: this.results.endTime.toISOString()
        };

        // 保存报告到文件
        const reportPath = path.join(__dirname, 'test-report.json');
        fs.writeFileSync(reportPath, JSON.stringify(report, null, 2));

        // 输出控制台报告
        console.log('\n=== 测试结果汇总 ===');
        console.log(`总计: ${report.summary.total}`);
        console.log(`通过: ${report.summary.passed}`);
        console.log(`失败: ${report.summary.failed}`);
        console.log(`成功率: ${report.summary.successRate}`);
        console.log(`执行时间: ${report.summary.duration}`);
        console.log(`报告已保存至: ${reportPath}`);

        return report;
    }
}

// 工具函数：创建截图目录
function ensureScreenshotDir() {
    if (!fs.existsSync(TEST_CONFIG.screenshotDir)) {
        fs.mkdirSync(TEST_CONFIG.screenshotDir, { recursive: true });
    }
}

async function safeJsClick(driver, element) {
    await driver.executeScript('arguments[0].click();', element);
}

async function tryClosePopups(driver) {
    const popupCloseLocators = [
        By.css('.login-close'),
        By.css('.close-icon'),
        By.css("[aria-label='close']"),
        By.xpath("//i[contains(@class,'close')]")
    ];

    for (const locator of popupCloseLocators) {
        try {
            const els = await driver.findElements(locator);
            for (const el of els) {
                try {
                    if (await el.isDisplayed()) {
                        await safeJsClick(driver, el);
                        await smartWait(300);
                    }
                } catch (_) {
                    // ignore
                }
            }
        } catch (_) {
            // ignore
        }
    }
}

// 工具函数：截图
async function takeScreenshot(driver, testName) {
    try {
        ensureScreenshotDir();
        const timestamp = new Date().toISOString().replace(/[:.]/g, '-');
        const filename = `${testName}_${timestamp}.png`;
        const filepath = path.join(TEST_CONFIG.screenshotDir, filename);

        const image = await driver.takeScreenshot();
        fs.writeFileSync(filepath, image, 'base64');

        console.log(`截图已保存: ${filepath}`);
        return filepath;
    } catch (error) {
        console.error('截图失败:', error.message);
        return null;
    }
}

// 工具函数：多策略元素定位
async function findElementByMultipleStrategies(driver, strategies, timeout = TEST_CONFIG.timeout.default) {
    const errors = [];

    for (const strategy of strategies) {
        try {
            const element = await driver.wait(until.elementLocated(strategy.locator), timeout);
            await driver.wait(until.elementIsVisible(element), timeout);
            return element;
        } catch (error) {
            errors.push(`${strategy.name}: ${error.message}`);
        }
    }

    throw new Error(`所有定位策略失败:\n${errors.join('\n')}`);
}

// 工具函数：等待元素并点击（带重试）
async function waitAndClick(driver, strategies, testName, timeout = TEST_CONFIG.timeout.default) {
    let lastError;

    for (let attempt = 1; attempt <= TEST_CONFIG.retries; attempt++) {
        try {
            await tryClosePopups(driver);

            const element = await findElementByMultipleStrategies(driver, strategies, timeout);
            await driver.executeScript("arguments[0].scrollIntoView({behavior: 'instant', block: 'center'});", element);
            await driver.wait(until.elementIsEnabled(element), timeout);

            try {
                await element.click();
            } catch (error) {
                await safeJsClick(driver, element);
            }

            return element;
        } catch (error) {
            lastError = error;
            if (attempt === TEST_CONFIG.retries) {
                await takeScreenshot(driver, `${testName}_fail`);
            }
            console.log(`尝试 ${attempt}/${TEST_CONFIG.retries} 失败: ${error.message}`);
            if (attempt < TEST_CONFIG.retries) {
                await smartWait(1000);
            }
        }
    }

    throw lastError;
}

// 工具函数：等待元素并输入文本（带重试）
async function waitAndType(driver, strategies, text, testName, timeout = TEST_CONFIG.timeout.default) {
    let lastError;

    for (let attempt = 1; attempt <= TEST_CONFIG.retries; attempt++) {
        try {
            await tryClosePopups(driver);

            const element = await findElementByMultipleStrategies(driver, strategies, timeout);
            await driver.executeScript("arguments[0].scrollIntoView({behavior: 'instant', block: 'center'});", element);
            await driver.wait(until.elementIsEnabled(element), timeout);
            await element.clear();
            await element.sendKeys(text);
            return element;
        } catch (error) {
            lastError = error;
            if (attempt === TEST_CONFIG.retries) {
                await takeScreenshot(driver, `${testName}_fail`);
            }
            console.log(`尝试 ${attempt}/${TEST_CONFIG.retries} 失败: ${error.message}`);
            if (attempt < TEST_CONFIG.retries) {
                await smartWait(1000);
            }
        }
    }

    throw lastError;
}

// 工具函数：智能等待
async function smartWait(ms = 2000) {
    await new Promise(resolve => setTimeout(resolve, ms));
}

// 工具函数：等待页面加载完成
async function waitForPageLoad(driver, timeout = TEST_CONFIG.timeout.long) {
    await driver.wait(async () => {
        const readyState = await driver.executeScript("return document.readyState");
        return readyState === "complete";
    }, timeout);
}

(async function anjukeTest() {
    console.log("=== 安居客功能测试开始 ===");
    let driver;
    const testResults = new TestResultCollector();

    try {
        // 初始化驱动
        driver = await new Builder().forBrowser('chrome').build();
        testResults.addResult("Chrome驱动初始化", "PASS");

        // 最大化窗口
        await driver.manage().window().maximize();
        testResults.addResult("窗口最大化", "PASS");

        // 设置隐式等待
        await driver.manage().setTimeouts({ implicit: TEST_CONFIG.timeout.default });

        // 导航到安居客南京租房页面
        console.log(`正在导航到: ${TEST_CONFIG.baseUrl}`);
        await driver.get(TEST_CONFIG.baseUrl);

        // 验证页面加载
        await waitForPageLoad(driver);
        await driver.wait(until.titleContains('安居客'), TEST_CONFIG.timeout.long);
        testResults.addResult("页面加载", "PASS");

        // 步骤3: 点击"地铁找房"
        await executeStep("地铁找房", async () => {
            const strategies = [
                { name: "文本链接", locator: By.xpath("//a[contains(text(), '地铁找房')]") },
                { name: "CSS选择器", locator: By.css("a[href*='subway']") },
                { name: "类名选择", locator: By.css(".subway-search") }
            ];
            await waitAndClick(driver, strategies, "地铁找房");
            await smartWait(2000);
        }, testResults);

        // 步骤4: 选择"2号线"
        await executeStep("选择2号线", async () => {
            const strategies = [
                { name: "文本链接", locator: By.xpath("//a[contains(text(), '2号线')]") },
                { name: "数据属性", locator: By.xpath("//a[data-line='2']") },
                { name: "类名选择", locator: By.css(".line-2") }
            ];
            await waitAndClick(driver, strategies, "2号线");
            await smartWait(2000);
        }, testResults);

        // 步骤5: 选择"马群"
        await executeStep("选择马群", async () => {
            const strategies = [
                { name: "文本链接", locator: By.xpath("//a[contains(text(), '马群')]") },
                { name: "站点数据", locator: By.xpath("//a[data-station='马群']") },
                { name: "类名选择", locator: By.css(".station-maqun") }
            ];
            await waitAndClick(driver, strategies, "马群");
            await smartWait(2000);
        }, testResults);

        // 步骤6: 设置租金5000-8000元
        await executeStep("设置租金5000-8000元", async () => {
            await smartWait(3000); // 等待筛选区域加载

            // 页面上常见的“租金”筛选行：右侧有两个小输入框 (min/max)
            // 优先在包含“租金”字样的区域内查找输入框，避免误选顶部搜索框
            const rentRowMinInput = By.xpath(
                "(//*[self::li or self::div][.//*[contains(normalize-space(.),'租金')]]//input[not(@type='hidden') and (not(@type) or @type='text' or @type='number')])[1]"
            );
            const rentRowMaxInput = By.xpath(
                "(//*[self::li or self::div][.//*[contains(normalize-space(.),'租金')]]//input[not(@type='hidden') and (not(@type) or @type='text' or @type='number')])[2]"
            );

            // 兜底策略：如果页面结构变化，退化为页面上靠前的两个输入框
            // 注意：这个策略更脆弱，但能在 DOM 属性缺失时救场
            const fallbackMinInput = By.xpath("(//input[not(@type='hidden') and (@type='text' or @type='number')])[1]");
            const fallbackMaxInput = By.xpath("(//input[not(@type='hidden') and (@type='text' or @type='number')])[2]");

            const minPriceStrategies = [
                { name: "租金行-第1个输入框", locator: rentRowMinInput },
                { name: "通用-第1个输入框", locator: fallbackMinInput }
            ];

            const maxPriceStrategies = [
                { name: "租金行-第2个输入框", locator: rentRowMaxInput },
                { name: "通用-第2个输入框", locator: fallbackMaxInput }
            ];

            await waitAndType(driver, minPriceStrategies, '5000', "最低租金");
            await waitAndType(driver, maxPriceStrategies, '8000', "最高租金");

            const confirmStrategies = [
                { name: "确定按钮", locator: By.xpath("//button[contains(text(), '确定')]") },
                { name: "提交按钮", locator: By.xpath("//input[@value='确定']") },
                { name: "链接确定", locator: By.xpath("//a[contains(text(), '确定')]") }
            ];

            try {
                await waitAndClick(driver, confirmStrategies, "确认租金");
            } catch (error) {
                console.log("确认按钮未找到，尝试按Enter键");
                const maxPriceInput = await findElementByMultipleStrategies(driver, maxPriceStrategies);
                await maxPriceInput.sendKeys(Key.ENTER);
            }

            await smartWait(2000);
        }, testResults);

        // 步骤7: 选择"整租"
        await executeStep("选择整租", async () => {
            const strategies = [
                { name: "文本链接", locator: By.xpath("//a[contains(text(), '整租')]") },
                { name: "整租选项", locator: By.css(".rent-type-whole") },
                { name: "数据属性", locator: By.xpath("//a[data-rent-type='whole']") }
            ];
            await waitAndClick(driver, strategies, "整租");
            await smartWait(2000);
        }, testResults);

        // 步骤8: 选择"普通住宅"
        await executeStep("选择普通住宅", async () => {
            try {
                // 策略1: 尝试直接点击
                const directStrategies = [
                    { name: "直接链接", locator: By.xpath("//a[contains(text(),'普通住宅')]") }
                ];
                // 使用较短的超时时间尝试直接点击
                await waitAndClick(driver, directStrategies, "普通住宅", 3000);
            } catch (e) {
                console.log("直接点击未成功，尝试通过下拉菜单选择...");

                // 策略2: 先点击下拉菜单（通常是"类型"或"更多"）
                const dropdownStrategies = [
                    { name: "类型/更多下拉", locator: By.xpath("//li[contains(text(),'类型') or contains(text(),'更多')] | //div[contains(@class,'filter')]//i[contains(@class,'arrow')]") }
                ];
                await waitAndClick(driver, dropdownStrategies, "下拉菜单");
                await smartWait(1000);

                // 然后点击选项
                const optionStrategies = [
                    { name: "下拉选项", locator: By.xpath("//a[contains(text(),'普通住宅')]") }
                ];
                await waitAndClick(driver, optionStrategies, "普通住宅(下拉)");
            }
            await smartWait(1500);
        }, testResults);

        // 步骤9: 搜索"经天路"
        await executeStep("搜索经天路", async () => {
            const searchStrategies = [
                { name: "搜索输入框", locator: By.css("input[type='text']") },
                { name: "搜索框类名", locator: By.css(".search-input") },
                { name: "搜索框ID", locator: By.css("#search-input") }
            ];

            const searchInput = await findElementByMultipleStrategies(driver, searchStrategies);
            await searchInput.clear();
            await searchInput.sendKeys('经天路');

            const btnStrategies = [
                { name: "搜索按钮", locator: By.xpath("//button[contains(text(), '搜索')]") },
                { name: "提交按钮", locator: By.xpath("//input[@type='submit']") },
                { name: "搜索图标", locator: By.xpath("//i[contains(@class, 'search')]") }
            ];

            await waitAndClick(driver, btnStrategies, "搜索按钮");
            await smartWait(3000);
        }, testResults);

        // 步骤10: 选择"视频看房"
        await executeStep("选择视频看房", async () => {
            const strategies = [
                { name: "视频看房链接", locator: By.xpath("//a[contains(text(), '视频看房')]") },
                { name: "视频选项", locator: By.xpath("//a[contains(text(), '视频')]") },
                { name: "视频筛选", locator: By.css(".filter-video") }
            ];
            await waitAndClick(driver, strategies, "视频看房");
            await smartWait(2000);
        }, testResults);

        // 步骤11: 排序功能测试
        await executeStep("测试排序功能", async () => {
            // 按租金排序
            const rentSortStrategies = [
                { name: "租金排序", locator: By.xpath("//a[contains(text(), '租金')]") },
                { name: "价格排序", locator: By.css(".sort-price") }
            ];
            await waitAndClick(driver, rentSortStrategies, "租金排序");
            await smartWait(2000);

            // 按最新排序
            const latestSortStrategies = [
                { name: "最新排序", locator: By.xpath("//a[contains(text(), '最新')]") },
                { name: "时间排序", locator: By.css(".sort-latest") }
            ];
            await waitAndClick(driver, latestSortStrategies, "最新排序");
            await smartWait(2000);
        }, testResults);

        // 步骤12: 点击第一个搜索结果
        await executeStep("点击第一个搜索结果", async () => {
            const strategies = [
                { name: "房源链接", locator: By.xpath("(//div[contains(@class, 'zu-itemmod')]//a)[1]") },
                { name: "项目链接", locator: By.xpath("(//div[contains(@class, 'item')]//a)[1]") },
                { name: "列表项", locator: By.css(".house-item a") }
            ];

            const firstResult = await findElementByMultipleStrategies(driver, strategies);
            const firstUrl = await firstResult.getAttribute("href");
            console.log(`准备打开房源链接: ${firstUrl}`);

            await firstResult.click();

            // 处理可能的新窗口
            const handles = await driver.getAllWindowHandles();
            if (handles.length > 1) {
                await driver.switchTo().window(handles[handles.length - 1]);
                console.log("✓ 切换到新窗口");
            }

            await smartWait(3000);
        }, testResults);

    } catch (error) {
        console.error("!!! 测试执行错误:", error);
        testResults.addResult("测试执行错误", "FAIL", error);
        if (driver) {
            await takeScreenshot(driver, "test_execution_error");
        }
    } finally {
        testResults.finish();

        if (driver) {
            await driver.quit();
            console.log("\n✓ 浏览器已关闭");
        }
    }
})();

// 辅助函数：执行测试步骤
async function executeStep(stepName, stepFunction, testResults) {
    console.log(`\n步骤: ${stepName}`);
    try {
        await stepFunction();
        testResults.addResult(stepName, "PASS");
    } catch (error) {
        testResults.addResult(stepName, "FAIL", error);
    }
}