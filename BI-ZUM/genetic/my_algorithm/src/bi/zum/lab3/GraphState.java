/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bi.zum.lab3;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Queue;
import cz.cvut.fit.zum.data.Edge;
import cz.cvut.fit.zum.api.Node;
import cz.cvut.fit.zum.data.StateSpace;
import java.util.Random;
import java.util.Collections;
        

/**
 *
 * @author Jaroslav Urban
 */
public class GraphState {
    private static List<Node> NodesState;
    private static List<Edge> EdgesState;
    private static Boolean[] isNodeInState;
    private static Queue<Node> nextNodes;
    private static Random rand;
    
    public static void init(int nodesToAdd) {
        NodesState = new ArrayList();
        EdgesState = new ArrayList();
        isNodeInState = new Boolean[StateSpace.nodesCount()];
        nextNodes = new LinkedList();
        rand = new Random();
        nextNodes.add(StateSpace.getNode(0));
        Arrays.fill(isNodeInState, Boolean.FALSE);
        enlargeState(nodesToAdd);
    }
    
    public static List<Edge> getEdges(){
        return EdgesState;
    }
    
    public static List<Node> getNodes() {
        return NodesState;
    }
        
    public static boolean isInState(int n){
        return isNodeInState[n];
    }
        
    private static void addNode(Node n){
        // adds node to state
        NodesState.add(n);
        isNodeInState[n.getId()] = true;
        
        // adds edges that have both nodes in state to state
        for (Edge e: n.getEdges()){
            if (!isNodeInState[e.getFromId()]) {
                nextNodes.add(StateSpace.getNode(e.getFromId()));
            } else if (!isNodeInState[e.getToId()]){
                nextNodes.add(StateSpace.getNode(e.getToId()));
            } else{
                EdgesState.add(e);
            }
        }
    }
    
    // adds numberOfNodes to state
    public static void enlargeState(int numberOfNodes){
        Node n;
        for (int i = 0; !nextNodes.isEmpty() && i < numberOfNodes; i++){
            n = nextNodes.poll();
            if (isInState(n.getId())){
                --i; continue;
            }
            addNode(n);
        }
    }
}
