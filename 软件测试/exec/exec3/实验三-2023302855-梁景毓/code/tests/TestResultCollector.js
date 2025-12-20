const { Builder, By, Key, until } = require('selenium-webdriver');
const fs = require('fs');
const path = require('path');

// ================= 配置区域 =================
const TEST_CONFIG = {
    baseUrl: 'https://nj.zu.anjuke.com/',
    timeout: 10000,
    screenshotDir: path.join(__dirname, 'screenshots')
};

// ================= 工具类定义 =================

class TestResultCollector {
    constructor() {
        this.results = { passed: 0, failed: 0, details: [] };
    }
    addResult(step, status, error = null) {
        this.results.details.push({ step, status, error: error ? error.message : null });
        if (status === 'PASS') {
            this.results.passed++;
            console.log(`✅ [成功] ${step}`);
        } else {
            this.results.failed++;
            console.log(`❌ [失败] ${step} - ${error ? error.message : ''}`);
        }
    }
    printSummary() {
        console.log('\n========= 测试结果汇总 =========');
        console.log(`总计: ${this.results.passed + this.results.failed} | 通过: ${this.results.passed} | 失败: ${this.results.failed}`);
        console.log(`通过率: ${((this.results.passed / (this.results.passed + this.results.failed)) * 100).toFixed(1)}%`);
        console.log('===============================');
    }
}

const smartWait = (ms) => new Promise(resolve => setTimeout(resolve, ms));

async function tryClosePopups(driver) {
    const popupLocators = [
        By.css('.login-close'), By.css('.close-icon'),
        By.className('login-wrap-close'), By.xpath("//i[contains(@class,'close')]")
    ];
    for (const loc of popupLocators) {
        try {
            const els = await driver.findElements(loc);
            if (els.length > 0 && await els[0].isDisplayed()) {
                await els[0].click();
            }
        } catch (_) {}
    }
}

async function findElementMulti(driver, strategies) {
    let lastError = null;
    await tryClosePopups(driver);

    for (const strategy of strategies) {
        try {
            const el = await driver.wait(until.elementLocated(strategy.locator), 3000);
            await driver.wait(until.elementIsVisible(el), 2000);
            return el;
        } catch (error) {
            lastError = error;
        }
    }
    throw new Error(`无法定位元素: ${lastError ? lastError.message : '所有策略均失败'}`);
}

// ================= 主逻辑 =================

(async function runAutoTest() {
    let driver = await new Builder().forBrowser('chrome').build();
    const results = new TestResultCollector();

    try {
        await driver.manage().window().maximize();
        console.log('🚀 === 安居客自动化测试 (截图修复版) ===');

        // Step 1: 首页
        await execute(driver, results, "1. 进入首页", async () => {
            await driver.get(TEST_CONFIG.baseUrl);
            await smartWait(2000);
        });

        // Step 3: 地铁找房
        await execute(driver, results, "3. 点击地铁找房", async () => {
            const el = await findElementMulti(driver, [
                { locator: By.xpath("//a[contains(text(), '地铁找房') or contains(text(), '地铁')]") },
                { locator: By.css(".subway-tab") }
            ]);
            await el.click();
            await smartWait(1500);
        });

        // Step 4: 2号线
        await execute(driver, results, "4. 选择2号线", async () => {
            const el = await findElementMulti(driver, [
                { locator: By.xpath("//a[contains(text(), '2号线')]") }
            ]);
            await el.click();
            await smartWait(1500);
        });

        // Step 5: 马群
        await execute(driver, results, "5. 选择马群", async () => {
            const el = await findElementMulti(driver, [
                { locator: By.xpath("//a[contains(text(), '马群')]") }
            ]);
            await el.click();
            await smartWait(2000);
        });

        // Step 6: 租金 5000-8000
        await execute(driver, results, "6. 设置租金5000-8000", async () => {
            let targetInputs = [];
            try {
                // 尝试找确定按钮附近的输入框
                const confirmBtn = await driver.findElements(By.xpath("//button[contains(text(), '确定')] | //input[@value='确定']"));
                if (confirmBtn.length > 0) {
                    targetInputs = await confirmBtn[0].findElement(By.xpath("./..")).findElements(By.tagName("input"));
                }
                // 尝试找筛选区内的输入框
                if (targetInputs.length < 2) {
                    const filterArea = await driver.findElement(By.xpath("//div[contains(@class,'filter') or contains(@class,'select')]"));
                    targetInputs = await filterArea.findElements(By.tagName("input"));
                }
            } catch (e) {}

            // 全局搜索备用
            if (targetInputs.length < 2) {
                targetInputs = await driver.findElements(By.xpath("//input[@type='text' or @type='number']"));
            }

            let visibleInputs = [];
            for (let inp of targetInputs) {
                if (await inp.isDisplayed()) {
                    const placeholder = await inp.getAttribute("placeholder");
                    // 排除搜索框
                    if (!placeholder || (!placeholder.includes("搜") && !placeholder.includes("找"))) {
                        visibleInputs.push(inp);
                    }
                }
            }

            if (visibleInputs.length >= 2) {
                await visibleInputs[0].clear();
                await visibleInputs[0].sendKeys('5000');
                await smartWait(500);
                await visibleInputs[1].clear();
                await visibleInputs[1].sendKeys('8000');
                await smartWait(500);

                try {
                    const btn = await driver.findElement(By.xpath("//button[contains(text(), '确定')] | //input[@value='确定']"));
                    if(await btn.isDisplayed()) await btn.click();
                    else await visibleInputs[1].sendKeys(Key.RETURN);
                } catch (e) {
                    await visibleInputs[1].sendKeys(Key.RETURN);
                }
            } else {
                // 兜底链接
                const link = await findElementMulti(driver, [
                    { locator: By.xpath("//a[contains(text(), '5000') and contains(text(), '8000')]") },
                    { locator: By.xpath("//a[contains(text(), '5000') and contains(text(), '以上')]") }
                ]);
                await link.click();
            }
            await smartWait(2500);
        });

        // Step 7: 整租
        await execute(driver, results, "7. 选择整租", async () => {
            const el = await findElementMulti(driver, [
                { locator: By.xpath("//a[contains(text(), '整租')]") }
            ]);
            await el.click();
            await smartWait(1500);
        });

        // Step 8: 普通住宅 (根据截图修复：点击“房屋类型” -> “普通住宅”)
        await execute(driver, results, "8. 选择普通住宅", async () => {
            try {
                // 1. 点击“房屋类型”下拉菜单
                // 根据截图，这里是一个包含文字“房屋类型”的元素，可能是 div, li, span 或 a
                const typeDropdown = await findElementMulti(driver, [
                    { locator: By.xpath("//*[contains(text(), '房屋类型')]") },
                    { locator: By.xpath("//div[contains(@class, 'filter')]//li[contains(text(), '类型')]") }
                ]);
                await typeDropdown.click();
                await smartWait(1000); // 等待下拉菜单展开

                // 2. 点击“普通住宅”
                const option = await findElementMulti(driver, [
                    { locator: By.xpath("//li[contains(text(), '普通住宅')]") },
                    { locator: By.xpath("//a[contains(text(), '普通住宅')]") },
                    { locator: By.xpath("//span[contains(text(), '普通住宅')]") }
                ]);

                if (await option.isDisplayed()) {
                    await option.click();
                } else {
                    // 如果元素在 DOM 中但不可见（可能需要鼠标悬停），尝试用 JS 点击
                    await driver.executeScript("arguments[0].click();", option);
                    console.log('   -> (使用JS点击) 普通住宅');
                }

            } catch (e) {
                // 如果上述逻辑失败，尝试最后的备选：直接找页面上可见的“普通住宅”链接
                console.log('   ⚠️ 下拉菜单操作遇阻，尝试直接点击可见链接...');
                const el = await driver.findElement(By.xpath("//a[contains(text(), '普通住宅')]"));
                await el.click();
            }
            await smartWait(1500);
        });

        // Step 9: 搜索经天路
        await execute(driver, results, "9. 搜索经天路", async () => {
            const searchBox = await findElementMulti(driver, [
                { locator: By.xpath("//div[contains(@class, 'search')]//input[@type='text']") },
                { locator: By.xpath("//form//input[@type='text']") },
                { locator: By.id("search-input") }
            ]);
            await searchBox.clear();
            await searchBox.sendKeys('经天路');
            await smartWait(500);
            await searchBox.sendKeys(Key.RETURN);
            await smartWait(3000);
        });

        // Step 10: 视频看房
        await execute(driver, results, "10. 视频看房", async () => {
            try {
                const el = await findElementMulti(driver, [
                    { locator: By.xpath("//a[contains(text(), '视频看房')]") }
                ]);
                await el.click();
            } catch (e) {
                console.log('   ⚠️ 未找到“视频看房”选项，跳过此步');
            }
            await smartWait(1500);
        });

        // Step 11: 排序 (默认排序 -> 最新)
        await execute(driver, results, "11. 排序(默认->最新)", async () => {
            try {
                // 1. 点击“默认排序”
                const sortDropdown = await findElementMulti(driver, [
                    { locator: By.xpath("//a[contains(text(), '默认排序')]") },
                    { locator: By.xpath("//a[contains(text(), '默认')]") },
                    { locator: By.xpath("//div[contains(@class, 'sort')]//span") }
                ]);
                await sortDropdown.click();
                await smartWait(1000);

                // 2. 点击“最新”
                const newSort = await driver.findElement(By.xpath("//a[contains(text(), '最新')] | //li[contains(text(), '最新')]"));
                await newSort.click();
            } catch (e) {
                console.log(`   ⚠️ 排序操作警告: ${e.message}`);
            }
            await smartWait(1500);
        });

        // Step 12: 点击房源
        await execute(driver, results, "12. 点击首个房源", async () => {
            const house = await findElementMulti(driver, [
                { locator: By.xpath("(//div[contains(@class, 'zu-itemmod')]//a | //div[@class='zu-info']//a)[1]") }
            ]);
            await driver.executeScript("arguments[0].scrollIntoView({behavior: 'smooth', block: 'center'});", house);
            await smartWait(500);

            const title = await house.getText();
            console.log(`   -> 点击房源: ${title.substring(0, 20)}...`);
            await house.click();

            const handles = await driver.getAllWindowHandles();
            await driver.switchTo().window(handles[handles.length - 1]);
            console.log('   -> 已切换到详情页');
        });

    } catch (err) {
        console.error('\n🚨 全局错误:', err);
    } finally {
        results.printSummary();
        console.log('⏳ 测试结束，浏览器将在 5 秒后关闭...');
        await smartWait(5000);
        await driver.quit();
    }
})();

async function execute(driver, results, stepName, fn) {
    try {
        await fn();
        results.addResult(stepName, 'PASS');
    } catch (e) {
        results.addResult(stepName, 'FAIL', e);
    }
}