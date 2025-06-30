package Lab;

import javax.lang.model.type.NullType;
import java.util.ArrayList;

public class ProductCatalog {
    private ArrayList<Product> products;
    public ProductCatalog(){};
    public void addProduct(Product product){products.add(product);}
    public void removeProduct(Product product){products.remove(product);}
    public Product getProduct(String code){
        for (Product product:products){
            if(product.getCode().equals(code)){
                return product;
            }
        }
        return new Product(null,null,-1, null,null);
    }
    public Product getProduct(int index){
        return  products.get(index);
    }
    public int getNumberOfProducts(){
        int num=products.size();
        return num;
    }
}
