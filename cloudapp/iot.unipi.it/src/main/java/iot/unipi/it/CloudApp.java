package iot.unipi.it;

import java.util.ArrayList;
import java.io.*;
import org.eclipse.californium.core.*;
import org.eclipse.californium.core.coap.MediaTypeRegistry;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

public class CloudApp extends CoapServer {
    
    static public ArrayList<Resource> reslist = new ArrayList<Resource>();
    
    static String commandLine;
	static BufferedReader console = new BufferedReader (new InputStreamReader(System.in));
    
	public static void main(String[] args) {
        CaliforniumLogger.disableLogging();        
        
        CloudApp server = new CloudApp();
        server.add(new CoAPRes("registration"));
    	server.start();
    	
		while(true){
		            
			System.out.print("Shell> ");
			
			try {

				commandLine = console.readLine();

				if (commandLine.equals(""))
					continue;
					
				if (commandLine.equals("help")){

					System.out.println();
					System.out.println();
					System.out.println("Welcome to the shell");
					System.out.println("--------------------");
					System.out.println();
					System.out.println("Commands to use:");
					System.out.println("1) help");
					System.out.println("2) exit");
					System.out.println("3) reslist");
					System.out.println("4) rename");
					System.out.println("5) history");
					System.out.println("6) get");
					System.out.println("7) post");
					System.out.println();
					System.out.println("---------------------");
					System.out.println();
				}
				
				if (commandLine.equals("exit")){
					
					System.out.println("Ciao Ciao");
					System.exit(0);
				}

				if (commandLine.equals("reslist")){

					for (Resource res : reslist){
						System.out.print("Name: " + res.getName());
						System.out.println(" | Index: " + reslist.indexOf(res));
					}					
				}

				if (commandLine.equals("rename")){

					System.out.println("insert id");
					System.out.print("ID: ");
					int id = Integer.parseInt(console.readLine());

					if (id >= reslist.size() || id < 0){
						System.out.println("Wrong index");
						continue;
					}
					
					System.out.println("insert new name");
					System.out.print("Name: ");
					String name = console.readLine();
					
					reslist.get(id).setName(name);
				}

				if (commandLine.equals("history")){

					System.out.println("insert id");
					System.out.print("ID: ");
					int id = Integer.parseInt(console.readLine());

					if (id >= reslist.size() || id < 0){
						System.out.println("Wrong index");
						continue;
					}

					if (reslist.get(id).isObservable()){
						System.out.println("Queue: " + reslist.get(id).getQueueObserve());
					} else {
						System.out.println("Resource not obs");
						continue;
					}					
				}

				if (commandLine.equals("get")){

					System.out.println("insert id");
					System.out.print("ID: ");
					int id = Integer.parseInt(console.readLine());

					if (id >= reslist.size() || id < 0){
						System.out.println("Wrong index");
						continue;
					}

					if (reslist.get(id).isObservable()){
						System.out.println("Value: " + reslist.get(id).getQueueObserve().getFirst());
					} else {

						String res = CloudApp.reslist.get(id).get(MediaTypeRegistry.APPLICATION_JSON).getResponseText();

						JSONParser parser = new JSONParser();

						try {
							JSONObject jsonObject = (JSONObject) parser.parse(res);
							for (Object o : jsonObject.keySet()) {
								System.out.println("Field \"" + o + "\" is: " + (jsonObject.get((String) o)));
							}
						} catch (ParseException ex) {
							ex.printStackTrace();
						}
					}
				}

				if (commandLine.equals("post")){

					System.out.println("insert id");
					System.out.print("ID: ");
					int id = Integer.parseInt(console.readLine());

					if (id >= reslist.size() || id < 0){
						System.out.println("Wrong index");
						continue;
					}

					if (reslist.get(id).hasMethod("POST")){
						String payload = new String();

						for (int i = 0; i < reslist.get(id).getFields().size(); i++){							
							System.out.print("Insert a value for " + reslist.get(id).getFields().get(i)[0]);
							if (reslist.get(id).getFields().get(i)[1].contains("<value>")){
								System.out.print(": ");
								payload = payload + "&value=" + console.readLine();
							} else {
								System.out.print(" between 'on' or 'off': ");
								String status = console.readLine();
								if (status.equals("on")){
									if (i == 0){
										payload = "status=" + status;
									} else {
										payload = payload + "&force=" + status;
									}
								} else if (status.equals("off")){
									if (i == 0){
										payload = "status=" + status;
									} else {
										payload = payload + "&force=" + status;
									}
									break;
								} else {
									System.out.println("Wrong value");
									break;
								}
							}
						}

						// System.out.println(payload);						
						reslist.get(id).post(payload, MediaTypeRegistry.TEXT_PLAIN);

					} else {
						System.out.println("Post not available for resource");
						continue;
					}
				}

			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
	}
	
}