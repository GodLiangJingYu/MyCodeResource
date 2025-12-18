const { Builder, By, Key, until } = require('selenium-webdriver');
const assert = require('assert');

// 工具函数：等待元素并点击
async function waitAndClick(driver, locator, timeout = 10000) {
    try {
        const element = await driver.wait(until.elementLocated(locator), timeout);
        await driver.wait(until.elementIsVisible(element), timeout);
        await element.click();
        return element;
    } catch (error) {
        console.error(`Error clicking element with locator ${locator}:`, error.message);
        throw error;
    }
}

// 工具函数：等待元素并输入文本
async function waitAndType(driver, locator, text, timeout = 10000) {
    try {
        const element = await driver.wait(until.elementLocated(locator), timeout);
        await driver.wait(until.elementIsVisible(element), timeout);
        await element.clear();
        await element.sendKeys(text);
        return element;
    } catch (error) {
        console.error(`Error typing in element with locator ${locator}:`, error.message);
        throw error;
    }
}

// 工具函数：智能等待
async function smartWait(driver, ms = 2000) {
    await new Promise(resolve => setTimeout(resolve, ms));
}

(async function anjukeTest() {
    console.log("=== 安居客功能测试开始 ===");
    let driver;
    let testResults = {
        passed: 0,
        failed: 0,
        details: []
    };
    
    try {
        // 初始化驱动
        driver = await new Builder().forBrowser('chrome').build();
        console.log("✓ Chrome驱动初始化成功");
        
        // 最大化窗口
        await driver.manage().window().maximize();
        console.log("✓ 窗口最大化完成");
        
        // 设置隐式等待
        await driver.manage().setTimeouts({ implicit: 10000 });
        
        // 导航到安居客南京租房页面
        const targetUrl = 'https://nj.zu.anjuke.com/';
        console.log(`正在导航到: ${targetUrl}`);
        await driver.get(targetUrl);
        
        // 验证页面加载
        await driver.wait(until.titleContains('安居客'), 15000);
        console.log("✓ 页面加载成功");
        
        // 步骤3: 点击"地铁找房"
        console.log("\n步骤3: 点击地铁找房");
        try {
            await waitAndClick(driver, By.xpath("//a[contains(text(), '地铁找房')]"));
            console.log("✓ 地铁找房点击成功");
            testResults.passed++;
            testResults.details.push("地铁找房: 成功");
        } catch (error) {
            console.log("✗ 地铁找房点击失败:", error.message);
            testResults.failed++;
            testResults.details.push("地铁找房: 失败 - " + error.message);
        }
        
        // 步骤4: 选择"2号线"
        console.log("\n步骤4: 选择2号线");
        try {
            await waitAndClick(driver, By.xpath("//a[contains(text(), '2号线')]"));
            console.log("✓ 2号线选择成功");
            testResults.passed++;
            testResults.details.push("2号线选择: 成功");
        } catch (error) {
            console.log("✗ 2号线选择失败:", error.message);
            testResults.failed++;
            testResults.details.push("2号线选择: 失败 - " + error.message);
        }
        
        // 步骤5: 选择"马群"
        console.log("\n步骤5: 选择马群");
        try {
            await waitAndClick(driver, By.xpath("//a[contains(text(), '马群')]"));
            console.log("✓ 马群选择成功");
            testResults.passed++;
            testResults.details.push("马群选择: 成功");
        } catch (error) {
            console.log("✗ 马群选择失败:", error.message);
            testResults.failed++;
            testResults.details.push("马群选择: 失败 - " + error.message);
        }
        
        // 步骤6: 设置租金5000-8000元
        console.log("\n步骤6: 设置租金5000-8000元");
        try {
            await smartWait(driver, 3000); // 等待筛选区域加载
            
            // 尝试多种可能的定位策略
            let minPriceInput, maxPriceInput, confirmBtn;
            
            // 策略1: 通过placeholder查找
            try {
                minPriceInput = await driver.findElement(By.xpath("//input[contains(@placeholder, '最低') or contains(@placeholder, 'min')]"));
                maxPriceInput = await driver.findElement(By.xpath("//input[contains(@placeholder, '最高') or contains(@placeholder, 'max')]"));
            } catch (e) {
                // 策略2: 通过class或id查找价格输入框
                try {
                    const priceInputs = await driver.findElements(By.css("input[type='text'][class*='price'], input[class*='price']"));
                    if (priceInputs.length >= 2) {
                        minPriceInput = priceInputs[0];
                        maxPriceInput = priceInputs[1];
                    }
                } catch (e2) {
                    throw new Error("无法定位价格输入框");
                }
            }
            
            // 输入价格
            await minPriceInput.clear();
            await minPriceInput.sendKeys('5000');
            await maxPriceInput.clear();
            await maxPriceInput.sendKeys('8000');
            
            // 查找确认按钮
            try {
                confirmBtn = await driver.findElement(By.xpath("//button[contains(text(), '确定')] | //input[@value='确定'] | //a[contains(text(), '确定')]"));
                await confirmBtn.click();
            } catch (e) {
                console.log("确认按钮未找到，尝试按Enter键");
                await maxPriceInput.sendKeys(Key.ENTER);
            }
            
            await smartWait(driver, 2000);
            console.log("✓ 租金设置成功");
            testResults.passed++;
            testResults.details.push("租金设置: 成功");
        } catch (error) {
            console.log("✗ 租金设置失败:", error.message);
            testResults.failed++;
            testResults.details.push("租金设置: 失败 - " + error.message);
        }
        
        // 步骤7: 选择"整租"
        console.log("\n步骤7: 选择整租");
        try {
            await waitAndClick(driver, By.xpath("//a[contains(text(), '整租')]"));
            console.log("✓ 整租选择成功");
            testResults.passed++;
            testResults.details.push("整租选择: 成功");
        } catch (error) {
            console.log("✗ 整租选择失败:", error.message);
            testResults.failed++;
            testResults.details.push("整租选择: 失败 - " + error.message);
        }
        
        // 步骤8: 选择"普通住宅"
        console.log("\n步骤8: 选择普通住宅");
        try {
            await waitAndClick(driver, By.xpath("//a[contains(text(), '普通住宅')]"));
            console.log("✓ 普通住宅选择成功");
            testResults.passed++;
            testResults.details.push("普通住宅选择: 成功");
        } catch (error) {
            console.log("✗ 普通住宅选择失败:", error.message);
            testResults.failed++;
            testResults.details.push("普通住宅选择: 失败 - " + error.message);
        }
        
        // 步骤9: 搜索"经天路"
        console.log("\n步骤9: 搜索经天路");
        try {
            // 查找搜索框
            const searchInput = await driver.wait(until.elementLocated(By.css("input[type='text'], input.search-input")), 10000);
            await searchInput.clear();
            await searchInput.sendKeys('经天路');
            
            // 查找搜索按钮
            const searchBtn = await driver.findElement(By.xpath("//button[contains(text(), '搜索')] | //input[@type='submit'] | //i[contains(@class, 'search')]"));
            await searchBtn.click();
            
            await smartWait(driver, 3000);
            console.log("✓ 经天路搜索成功");
            testResults.passed++;
            testResults.details.push("经天路搜索: 成功");
        } catch (error) {
            console.log("✗ 经天路搜索失败:", error.message);
            testResults.failed++;
            testResults.details.push("经天路搜索: 失败 - " + error.message);
        }
        
        // 步骤10: 选择"视频看房"
        console.log("\n步骤10: 选择视频看房");
        try {
            await waitAndClick(driver, By.xpath("//a[contains(text(), '视频看房') or contains(text(), '视频')]"));
            console.log("✓ 视频看房选择成功");
            testResults.passed++;
            testResults.details.push("视频看房: 成功");
        } catch (error) {
            console.log("✗ 视频看房选择失败:", error.message);
            testResults.failed++;
            testResults.details.push("视频看房: 失败 - " + error.message);
        }
        
        // 步骤11: 排序功能测试
        console.log("\n步骤11: 测试排序功能");
        try {
            // 按租金排序
            await waitAndClick(driver, By.xpath("//a[contains(text(), '租金')]"));
            await smartWait(driver, 2000);
            
            // 按最新排序
            await waitAndClick(driver, By.xpath("//a[contains(text(), '最新')]"));
            await smartWait(driver, 2000);
            
            console.log("✓ 排序功能测试成功");
            testResults.passed++;
            testResults.details.push("排序功能: 成功");
        } catch (error) {
            console.log("✗ 排序功能测试失败:", error.message);
            testResults.failed++;
            testResults.details.push("排序功能: 失败 - " + error.message);
        }
        
        // 步骤12: 点击第一个搜索结果
        console.log("\n步骤12: 点击第一个搜索结果");
        try {
            const firstResult = await driver.wait(until.elementLocated(By.xpath("(//div[contains(@class, 'zu-itemmod')]//a)[1] | (//div[contains(@class, 'item')]//a)[1]")), 10000);
            
            // 获取链接信息用于验证
            const firstUrl = await firstResult.getAttribute("href");
            console.log(`准备打开房源链接: ${firstUrl}`);
            
            await firstResult.click();
            
            // 处理可能的新窗口
            const handles = await driver.getAllWindowHandles();
            if (handles.length > 1) {
                await driver.switchTo().window(handles[handles.length - 1]);
                console.log("✓ 切换到新窗口");
            }
            
            await smartWait(driver, 3000);
            console.log("✓ 第一个房源点击成功");
            testResults.passed++;
            testResults.details.push("第一个房源点击: 成功");
        } catch (error) {
            console.log("✗ 第一个房源点击失败:", error.message);
            testResults.failed++;
            testResults.details.push("第一个房源点击: 失败 - " + error.message);
        }
        
    } catch (error) {
        console.error("!!! 测试执行错误:", error);
        testResults.failed++;
        testResults.details.push("测试执行错误: " + error.message);
    } finally {
        // 输出测试结果
        console.log("\n=== 测试结果汇总 ===");
        console.log(`通过: ${testResults.passed}`);
        console.log(`失败: ${testResults.failed}`);
        console.log(`总计: ${testResults.passed + testResults.failed}`);
        
        console.log("\n详细结果:");
        testResults.details.forEach(detail => {
            console.log(`  - ${detail}`);
        });
        
        if (driver) {
            await driver.quit();
            console.log("\n✓ 浏览器已关闭");
        }
    }
})();