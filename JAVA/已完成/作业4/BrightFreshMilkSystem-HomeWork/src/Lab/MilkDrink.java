package Lab;

import java.util.Date;

public class MilkDrink extends Product{
    private String flavor;
    private String sugar;
    public MilkDrink(String code, String description, double price, Date productionDate,String shelfLide,String flavor,String sugar){
        super(code,description,price,productionDate,shelfLide);
        this.flavor=flavor;
        this.sugar=sugar;
    }
    public String getFlavor(){return flavor;}
    public String getSugar(){return sugar;}
}
