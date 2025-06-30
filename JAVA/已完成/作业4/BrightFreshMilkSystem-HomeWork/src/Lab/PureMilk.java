package Lab;

import java.util.Date;

public class PureMilk extends Product{
    private String countryOfOrign;
    private String butterfat;
    private String protein;
    public PureMilk(String code, String description, double price, Date productionDate, String shelLife,String countryOfOrign,String butterfat,String protein){
        super(code, description, price, productionDate, shelLife);
        this.countryOfOrign=countryOfOrign;
        this.butterfat=butterfat;
        this.protein=protein;
    }
    private String getCountryOfOrign(){return countryOfOrign;}
    private String getButterfat(){return butterfat;}
    private String getProtein(){return protein;}
}
