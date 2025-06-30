package Lab;

import java.util.ArrayList;
import java.util.Date;

public class Product {
    private String code;
    private String description;
    private double price;
    private Date productionDate;
    private String shelLife;
    private ArrayList<Product> products;
    public Product(String code,String description,double price,Date productionDate,String shelLife){
        this.code=code;
        this.description=description;
        this.price=price;
        this.productionDate=productionDate;
        this.shelLife=shelLife;
    }
    public String getCode(){return code;}
    public String getDescription(){return description;}
    public double getPrice(){return price;}
    public Date getProductionDate(){return getProductionDate();}
    public String getShelLife(){return shelLife;}
    public String toString() {
        return code + '-' + description + '-' + price + '+' + productionDate + shelLife;
    }
}
