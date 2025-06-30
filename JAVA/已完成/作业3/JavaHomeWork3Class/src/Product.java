import java.util.Date;

public class Product {
    private String code;
    private String description;
    private double price;
    private Date productionDate;
    private String shelfLife;
    public Product(String code,String description,double price,Date productionDate,String shelfLife){
        this.code=code;
        this.description=description;
        this.price=price;
        this.productionDate=productionDate;
        this.shelfLife=shelfLife;
    }
    public String getCode(){return code;}
    public String getDescription(){return description;}
    public double getPrice(){return price;}
    public Date getProductionDate(){return productionDate;}
    public String getShelfLife(){return shelfLife;}
    public boolean equals(Object object){
        return this==object;
    }
    public String toString(){
        return code+'_'+description+'_'+price+'_'+productionDate+'_'+shelfLife;
    }
}
