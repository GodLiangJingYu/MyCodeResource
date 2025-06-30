package Lab;

import java.util.Date;

public class BrightFreshMilkSystem {
    private StoreSales storeSales;
    private ProductCatalog productCatalog;

    public BrightFreshMilkSystem() {
        this.storeSales = new StoreSales();
        this.productCatalog = new ProductCatalog();
    }

    // 显示所有产品目录
    public void displayCatalog() {
        System.out.println("Product Catalog:");
        for (int i = 0; i < productCatalog.getNumberOfProducts(); i++) {
            Product product = productCatalog.getProduct(i);
            System.out.println("Code: " + product.getCode() + ", Description: " + product.getDescription() +
                    ", Price: $" + product.getPrice() + ", Production Date: " + product.getProductionDate() +
                    ", Shelf Life: " + product.getShelLife());
        }
    }

    // 显示所有订单中的产品信息
    public void displayProduct() {
        int i = 0;
        while (i < storeSales.getNumberOfOrders() && storeSales.getOrder(i) != null) {
            storeSales.getOrder(i).toString(); // 假设 Order 类中有一个 display() 方法
            i++;
        }
    }

    // 显示所有订单信息
    public void displayOrder() {
        storeSales.displayAllOrders();
    }

    // 添加产品到产品目录
    public void addProduct(String code, String description, double price, Date productionDate, String shelfLife) {
        Product product = new Product(code, description, price, productionDate, shelfLife);
        productCatalog.addProduct(product);
        System.out.println("Product added: " + product.getDescription());
    }

    // 从产品目录中移除产品
    public void removeProduct(String code) {
        Product product = productCatalog.getProduct(code);
        if (product.getPrice() != -1) { // 根据 getProduct 返回的默认产品对象进行检查
            productCatalog.removeProduct(product);
            System.out.println("Product removed: " + product.getDescription());
        } else {
            System.out.println("Product not found.");
        }
    }

    // 添加订单到销售记录中
    public void addOrder(Order order) {
        storeSales.addOrder(order);
        System.out.println("Order added. Total items: " + order.getNumberOfItems() + ", Total cost: $" + order.getTotalCost());
    }

    // 显示所有销售额
    public void displaySales() {
        double totalSales = 0;
        for (int i = 0; i < storeSales.getTotalOrders(); i++) {
            Order order = storeSales.getOrder(i);
            if (order != null) {
                totalSales += order.getTotalCost();
            }
        }
        System.out.println("Total Sales: $" + totalSales);
    }

    // 获取产品目录对象，以便在外部可以直接操作产品
    public ProductCatalog getProductCatalog() {
        return productCatalog;
    }

    // 获取销售记录对象，以便在外部可以直接操作订单
    public StoreSales getStoreSales() {
        return storeSales;
    }
}
