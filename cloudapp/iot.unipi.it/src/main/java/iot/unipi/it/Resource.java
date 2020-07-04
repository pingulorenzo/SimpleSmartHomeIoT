package iot.unipi.it;

import java.util.LinkedList;
import java.util.ArrayList;
import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapObserveRelation;

public class Resource extends CoapClient {
    private String addr, path, name, methods;
    private ArrayList<String[]> fields;
    private boolean observable = false;
    private CoAPHandler handlerObs;
    private CoapObserveRelation relation;

    public Resource(String addr, String content) {
        super();

        String[] contentsplit = content.split(";");

        this.addr = addr;
        path = contentsplit[0].substring(contentsplit[0].indexOf("<") + 1, contentsplit[0].indexOf(">"));
        name = contentsplit[1].substring(contentsplit[1].indexOf("=") + 2, contentsplit[1].lastIndexOf("\""));
        methods = contentsplit[2].substring(1);

        if(!content.contains("null")){

            fields = new ArrayList<String[]>();

            // Separo i campi con l'&
            String[] fieldslong = contentsplit[3].substring(1).split("&");
        
            // Separo i campi con l'=
            for (String field : fieldslong){
                fields.add(field.split("="));
            }
        }

        observable = content.contains("obs");
        setURI("coap://[" + this.addr + "]" + this.path);

        if (observable) {
            handlerObs = new CoAPHandler();
            relation = observe(handlerObs);
        }
    }

    public String getAddr() {
        return addr;
    }

    public String getPath() {
        return path;
    }

    public String getName() {
        return name;
    }

    public ArrayList<String[]> getFields() {
        return fields;
    }

    public boolean hasMethod(String method) {
        return this.methods.contains(method.toUpperCase());
    }

    public CoapObserveRelation getRelation() {
        return relation;
    }

    public void setName(String name) {
        this.name = name;
    }
    
    public boolean isObservable() {
        return this.observable;
    }

    public LinkedList<String> getQueueObserve() {
        return this.handlerObs.getQueue();
    }

}
