package Lab;

import java.util.ArrayList;

public class Order {
    private ArrayList<SaleItem> saleItems;

    public Order(){
        this.saleItems = new ArrayList<>();
    }

    public void addSaleItem(SaleItem saleItem){
        saleItems.add(saleItem);
    }

    public void removeSaleItem(SaleItem saleItem){
        saleItems.remove(saleItem);
    }

    public SaleItem getSaleItem(Product product){
        for (SaleItem saleItem : saleItems) {
            if (saleItem.getProduct().equals(product)) {
                return saleItem;
            }
        }
        return null; // 表示未找到
    }

    public SaleItem getSaleItem(int index){
        if (index >= 0 && index < saleItems.size()) {
            return saleItems.get(index);
        }
        return null; // 表示未找到
    }

    public int getNumberOfItems(){
        return saleItems.size();
    }

    public double getTotalCost() {
        double cost = 0;
        for (SaleItem saleItem : saleItems) {
            cost += saleItem.getQuantity() * saleItem.getProduct().getPrice();
        }
        return cost;
    }
}
