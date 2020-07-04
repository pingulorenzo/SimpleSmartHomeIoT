package iot.unipi.it;

import java.net.InetAddress;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.server.resources.CoapExchange;

public class CoAPRes extends CoapResource {
	
    public CoAPRes(String name) {
        super(name);
    }

    @Override
    public void handleGET(CoapExchange exchange) {
        exchange.accept();

        InetAddress addr = exchange.getSourceAddress();

        String uri = new String("coap://[" + addr.toString().substring(1) + "]:5683/.well-known/core");
        CoapClient req = new CoapClient(uri);

        String response = req.get().getResponseText().replace("</.well-known/core>;", "");

        for (String subres : response.split("\n")){

            Resource res = new Resource(addr.toString().substring(1), subres);

            // Evito doppioni (Stesso Indirizzo e stesso Path)
            for (int i = 0; i < CloudApp.reslist.size(); i++)
                if (res.getAddr().equals(CloudApp.reslist.get(i).getAddr()) && res.getPath().equals(CloudApp.reslist.get(i).getPath()))
                    continue;
            
            CloudApp.reslist.add(res);
            //System.out.println("Risorsa " + res.getName() + " registrata");
        }
    }
}
