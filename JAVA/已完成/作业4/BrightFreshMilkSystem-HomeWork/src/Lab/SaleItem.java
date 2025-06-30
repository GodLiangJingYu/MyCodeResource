package Lab;

public class SaleItem {
    private int quantity;
    private Product product;

    public SaleItem(int quantity, Product product) {
        this.quantity = quantity;
        this.product = product;
    }

    public Product getProduct() {
        return product;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int newQuantity) {
        if (newQuantity > 0) {
            this.quantity = newQuantity;
        }
    }

    public double getValue() {
        return quantity * product.getPrice();
    }
}
