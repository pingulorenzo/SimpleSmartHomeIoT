package iot.unipi.it;

import java.util.LinkedList;
import org.eclipse.californium.core.CoapHandler;
import org.eclipse.californium.core.CoapResponse;
import org.eclipse.californium.core.coap.MediaTypeRegistry;

public class CoAPHandler implements CoapHandler {
	
    private final int full = 20; // Dimensioni coda
    private LinkedList<String> obsQueue;

    public CoAPHandler() {
        this.obsQueue = new LinkedList<String>();
    }

    public LinkedList<String> getQueue() {
        return this.obsQueue;
    }

    public void onLoad(CoapResponse response) {
    	
        if (!response.getOptions().isContentFormat(MediaTypeRegistry.APPLICATION_JSON))
            return;

        if (this.obsQueue.size() == this.full) {
            this.obsQueue.removeLast();
        }

        String text = response.getResponseText();

        this.obsQueue.addFirst(text.substring(text.indexOf(":") + 1, text.length() - 1));
    }

    public void onError() {
        System.err.println("Error during obs");
    }

}
