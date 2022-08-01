package cfg;

import java.util.ArrayList;

public class CFG {
    ArrayList<String> lines;
    Node root;
    int lineNumber=0;

    public CFG(ArrayList<String> lines) {
        this.lines = lines;
    }

    private Node addChild(){
        Node statement = null;
        ArrayList<Node> branchNode = new ArrayList<>();

        while (true) {
            lineNumber++;
            if(lineNumber>=lines.size()) return null;

            if(this.lines.get(lineNumber).contains("else if")) {
                
            } else if (this.lines.get(lineNumber).contains("if")) {
                
            } else if (this.lines.get(lineNumber).contains("else")) {
                
            } else if (this.lines.get(lineNumber).contains("for")) {

            } else if (this.lines.get(lineNumber).contains("while") && ! this.lines.get(lineNumber).contains("}while")) {

            } else if (this.lines.get(lineNumber).contains("do")) {

            } else {
                
            }
            break;
        }
        return null;
    }
    public void graph(){
        this.root = new Node(0);
    }

}
