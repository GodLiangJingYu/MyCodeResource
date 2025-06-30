package Lab;

import java.util.ArrayList;

public class StoreSales {
    private ArrayList<Order> orders;
    public StoreSales(){}
    public void addOrder(Order order){
        orders.add(order);
    }
    public void removeOrder(Order order){
        orders.remove(order);
    }
    public Order getOrder(int index){
        return orders.get(index);
    }
    public int getNumberOfOrders(){
        int num=0;
        for (Order order:orders){
            num++;
        }
        return num;
    }
    public void displayAllOrders() {
        for (Order order : orders) {
            System.out.println("Order Total Cost: $" + order.getTotalCost() + ", Number of Items: " + order.getNumberOfItems());
            // 如果你想显示每个 SaleItem，可以在这里遍历 saleItems
            for (int i = 0; i < order.getNumberOfItems(); i++) {
                SaleItem saleItem = order.getSaleItem(i);
                System.out.println(" - Product Code: " + saleItem.getProduct().getCode() + ", Quantity: " + saleItem.getQuantity());
            }
        }
    }
    public int getTotalOrders() {
        return orders.size();
    }

}
