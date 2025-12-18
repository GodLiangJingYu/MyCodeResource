/**
 * 安居客 Web 端功能测试自动化脚本
 * 修复版 - 纯净无引用标记
 */

const { Builder, By, Key, until } = require('selenium-webdriver');
const chrome = require('selenium-webdriver/chrome');
const fs = require('fs');
const path = require('path');

// --- 基础配置 ---
const CONFIG = {
    baseUrl: 'https://nj.zu.anjuke.com/',
    timeout: 15000,
    screenshotDir: './screenshots'
};

// --- 辅助工具类 ---
class TestHelper {
    constructor(driver) {
        this.driver = driver;
    }

    // 格式化日志输出
    async log(step, status, msg = '') {
        const time = new Date().toLocaleTimeString();
        console.log(`[${time}] [${status}] ${step}: ${msg}`);
    }

    // 智能点击：包含等待、滚动、遮挡处理
    async click(locator, desc) {
        try {
            const el = await this.driver.wait(until.elementLocated(locator), CONFIG.timeout);
            await this.driver.wait(until.elementIsVisible(el), CONFIG.timeout);

            // 平滑滚动到元素中间
            await this.driver.executeScript("arguments[0].scrollIntoView({behavior: 'smooth', block: 'center'});", el);
            await this.driver.sleep(500);

            try {
                await el.click();
            } catch (err) {
                // 如果被遮挡（如广告），使用JS强制点击
                await this.driver.executeScript("arguments[0].click();", el);
                await this.log(desc, 'WARN', '常规点击失败，已使用JS强制点击');
            }
            await this.log(desc, 'PASS');
        } catch (e) {
            await this.takeScreenshot(`Error_${desc}`);
            await this.log(desc, 'FAIL', e.message.split('\n')[0]);
            throw e;
        }
    }

    // 智能输入
    async type(locator, text, desc) {
        try {
            const el = await this.driver.wait(until.elementLocated(locator), CONFIG.timeout);
            await this.driver.executeScript("arguments[0].scrollIntoView({behavior: 'smooth', block: 'center'});", el);
            await el.clear();
            await el.sendKeys(text);
            await this.log(desc, 'PASS', `输入: ${text}`);
        } catch (e) {
            await this.log(desc, 'FAIL', e.message);
            throw e;
        }
    }

    // 截图功能
    async takeScreenshot(name) {
        if (!fs.existsSync(CONFIG.screenshotDir)) fs.mkdirSync(CONFIG.screenshotDir);
        const img = await this.driver.takeScreenshot();
        const filename = path.join(CONFIG.screenshotDir, `${name}_${Date.now()}.png`);
        fs.writeFileSync(filename, img, 'base64');
    }

    // 尝试关闭常见的干扰弹窗
    async closePopups() {
        const selectors = [
            By.css('.login-close'),
            By.css('.close-icon'),
            By.xpath("//i[contains(@class,'close')]")
        ];
        for (let sel of selectors) {
            try {
                const el = await this.driver.findElement(sel);
                if (await el.isDisplayed()) {
                    await el.click();
                    await this.driver.sleep(500);
                }
            } catch (e) {} // 忽略找不到的情况
        }
    }
}

// --- 主程序 ---
(async function runTest() {
    // 1. 配置 Chrome 选项
    const options = new chrome.Options();
    options.addArguments('--disable-blink-features=AutomationControlled'); // 隐藏 Selenium 特征
    options.addArguments('--start-maximized'); // 最大化窗口
    options.setUserAgent('Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36');

    let driver = await new Builder().forBrowser('chrome').setChromeOptions(options).build();
    const t = new TestHelper(driver);

    try {
        console.log('=== 安居客功能测试开始 ===');

        // 1. 打开安居客网页版
        await driver.get(CONFIG.baseUrl);
        await t.log('Step 1&2', 'PASS', '打开安居客南京租房首页');
        await t.closePopups();

        // 3. 点击“地铁找房”
        await t.click(By.xpath("//a[contains(text(),'地铁找房')]"), 'Step 3: 点击地铁找房');
        await driver.sleep(1500);

        // 4. 选择“2号线”
        await t.click(By.xpath("//a[contains(text(),'2号线')][1]"), 'Step 4: 选择2号线');
        await driver.sleep(1500);

        // 5. 选择“马群”
        await t.click(By.xpath("//a[contains(text(),'马群')]"), 'Step 5: 选择马群');
        await driver.sleep(1500);

        // 6. 设置租金为5000-8000元，并点击确定
        // 注意：根据页面实际情况，输入框可能有多个，这里尝试定位第1和第2个输入框
        const minInput = By.xpath("(//input[contains(@class,'input') or @type='text'])[1]");
        const maxInput = By.xpath("(//input[contains(@class,'input') or @type='text'])[2]");
        const confirmBtn = By.xpath("//button[contains(text(),'确定') or contains(text(),'OK')] | //input[@value='确定']");

        await t.type(minInput, '5000', 'Step 6.1: 输入最低租金');
        await t.type(maxInput, '8000', 'Step 6.2: 输入最高租金');
        await t.click(confirmBtn, 'Step 6.3: 点击价格确定');
        await driver.sleep(2000);

        // 7. 选择“整租”
        await t.click(By.xpath("//a[contains(text(),'整租')][contains(@class,'item')] | //a[contains(text(),'整租')]"), 'Step 7: 选择整租');
        await driver.sleep(1500);

        // 8. 房屋类型选择“普通住宅”
        try {
            await t.click(By.xpath("//a[contains(text(),'普通住宅')]"), 'Step 8: 选择普通住宅');
        } catch (e) {
            await t.log('Step 8', 'RETRY', '尝试在下拉菜单中查找普通住宅...');
            const dropdown = await driver.findElement(By.xpath("//li[contains(text(),'类型') or contains(text(),'更多')]"));
            await dropdown.click();
            await driver.sleep(500);
            await t.click(By.xpath("//a[contains(text(),'普通住宅')]"), 'Step 8: 选择普通住宅 (下拉中)');
        }
        await driver.sleep(1500);

        // 9. 在搜索框中搜索“经天路”，并点击“搜索”
        const searchBox = By.xpath("//input[contains(@class,'search') or @type='text']");
        const searchBtn = By.xpath("//i[contains(@class,'search')]/.. | //button[contains(text(),'搜索')]");

        await t.type(searchBox, '经天路', 'Step 9.1: 输入关键字');
        await t.click(searchBtn, 'Step 9.2: 点击搜索按钮');
        await driver.sleep(3000);
        await t.closePopups();

        // 10. 选择“视频看房”
        await t.click(By.xpath("//a[contains(text(),'视频看房') or contains(text(),'视频')]"), 'Step 10: 选择视频看房');
        await driver.sleep(1500);

        // 11. 依次点击“租金”、“最新”排序
        await t.click(By.xpath("//a[contains(text(),'租金')]"), 'Step 11.1: 点击租金排序');
        await driver.sleep(1500);
        await t.click(By.xpath("//a[contains(text(),'最新')]"), 'Step 11.2: 点击最新排序');
        await driver.sleep(1500);

        // 12. 点击第一个搜索出来的房源进行查看
        const firstHouse = By.xpath("(//div[contains(@class,'zu-itemmod') or contains(@class,'list')]//a)[1]");
        const originalHandle = await driver.getWindowHandle();

        await t.click(firstHouse, 'Step 12: 点击第一个房源');

        // 等待新窗口并切换
        await driver.wait(async () => (await driver.getAllWindowHandles()).length > 1, 5000);
        const handles = await driver.getAllWindowHandles();
        for (let h of handles) {
            if (h !== originalHandle) {
                await driver.switchTo().window(h);
                break;
            }
        }

        // 验证最终结果
        const title = await driver.getTitle();
        await t.log('验证', 'PASS', `进入详情页: ${title}`);
        await t.takeScreenshot('Final_Success');

    } catch (error) {
        console.error('\n!!! 测试异常终止 !!!');
        console.error(error);
    } finally {
        console.log('\n--- 测试完成，正在关闭浏览器 ---');
        if (driver) await driver.quit();
    }
})();


