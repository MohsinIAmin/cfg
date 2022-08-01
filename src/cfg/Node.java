package cfg;

import java.util.ArrayList;

public class Node {
    public ArrayList<Node> children = new ArrayList<>();
    ArrayList<Integer> statementNumbers = new ArrayList<>();

    public Node(int lineNumber) {
        statementNumbers.add(lineNumber);
    }
}
