import java.util.HashMap;

import static java.lang.Integer.parseInt;

public class ResponseAnalyzer {
    private final HashMap<Integer, String> errors;

    public ResponseAnalyzer() {
        receivedResponse = "";
        errors = new HashMap<Integer, String>();
        errors.put(100, "OK");
        errors.put(300, "TOO_LONG_LINE");
        errors.put(301, "TOO_MANY_FIELDS");
        errors.put(302, "BAD_FIELD");
        errors.put(303, "BAD_COMMAND");
        errors.put(401, "UNKNOWN");
        errors.put(402, "NOT_LOGGED_IN");
        errors.put(403, "INVALID_FIELD_VALUE");
        errors.put(404, "NOT_FOUND");
        errors.put(405, "TARGET_NOT_FOUND");
        errors.put(406, "NOT_ENOUGH_PERMS");
        errors.put(407, "USER_ALREADY_LOGGED");
        errors.put(408, "INVALID_CREDENTIALS");
        errors.put(409, "FILE_ALREADY_EXISTS");
        errors.put(410, "INVALID_FILE_TYPE");
        errors.put(411, "DLS_WITHOUT_DL");
        errors.put(412, "IN_USE");
        errors.put(413, "WRONG_UPLOAD_PATH");
        errors.put(414, "NOT_A_DIRECTORY");
        errors.put(415, "TOO_LARGE_FILE");


        dl=false;
        dls=false;
        ul=false;
    }
    public boolean dl;
    public boolean dls;
    public boolean ul;

    private String receivedResponse;
    private int code;
    public boolean isResponseClear(){
        return receivedResponse.isEmpty();
    }


    public void setCode() {
        this.code = parseInt(receivedResponse.substring(0,3));
    }


    private boolean isMessageComplete() {
        return receivedResponse.endsWith("\n\n");
    }

    private void addToReceivedResponse(String receivedResponse) {
        this.receivedResponse += receivedResponse;
    }
    private void clearReceivedResponse(){this.receivedResponse= "";
    }

    public boolean isCodeValid(JavaClientGUI fr) {
        setCode();
        if (errors.containsKey(code)) {
            if(code!=100) {

                fr.setTextArea("Try again. Error:");
                fr.appendTextArea(errors.get(code));
            }
            else{
                fr.setTextArea("OK \n");
            }
            return true;
        }
        return false;
    }


    private void analyzeLongResponse(String response,JavaClientGUI fr ) {

        //
        //dl dls - pobireanie plików
        //ul

        if (dl) {
            if (dls) {

            }
            else{
                String size = receivedResponse.substring(receivedResponse.indexOf("dl") + 3);
                System.out.println(size);
                System.out.println(receivedResponse);
            }
        }
        else if (ul) {


        }
        else {
            int begin = response.indexOf("\n") + 1;
            int finish = response.indexOf("\n\n");
            String display = "\n" + response.substring(begin, finish);
            String toReplace= " ";
            String replacement= "   ";
            display = display.replaceAll(toReplace,replacement);
            fr.appendTextArea(display);
        }

    }


    public void analyze(String response, JavaClientGUI fr) {
        addToReceivedResponse(response);
        System.out.println(receivedResponse);
        if (isMessageComplete()) {
            setCode();
            if (isCodeValid(fr)) {
                if (receivedResponse.substring(4, 6).equals("dl")) {
                    dl=true;
                }
                if (receivedResponse.substring(4, 6).equals("ul")) {
                    ul=true;
                }

                long nlines = receivedResponse.lines().count();
                if (nlines > 2 || dl || ul) {
                    analyzeLongResponse(receivedResponse, fr);
                }
            }
            int index = receivedResponse.indexOf("\n\n");
            String analyzedPart = receivedResponse.substring(0,index+2);
            receivedResponse=receivedResponse.replace(analyzedPart, "");
        }
    }
}