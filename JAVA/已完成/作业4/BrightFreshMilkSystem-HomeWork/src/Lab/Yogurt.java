package Lab;

import java.util.Date;

public class Yogurt extends Product{
    private String type;
    private String diluteConcentration;
    public Yogurt(String code, String description, double price, Date productionDate, String shelLife,String type,String diluteConcentration){
        super(code, description, price, productionDate, shelLife);
        this.type=type;
        this.diluteConcentration=diluteConcentration;
    }
    public String getType(){return type;}
    public String getDiluteConcentration(){return diluteConcentration;}
}
