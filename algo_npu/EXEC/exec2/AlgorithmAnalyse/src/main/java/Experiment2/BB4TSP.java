package Experiment2;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Vector;

public class BB4TSP {

 int NoEdge = -1; // no edge marker
 private int minCost = Integer.MAX_VALUE; // current minimal cost
 public int getMinCost() {
  return minCost;
 }

 public void setMinCost(int minCost) {
  this.minCost = minCost;
 }

 private LinkedList<HeapNode> heap = new LinkedList<HeapNode>();// store nodes (unused in DFS approach)
 private Vector<Integer> bestH = new Vector<Integer>();


 @SuppressWarnings("rawtypes")
 public static class HeapNode implements Comparable<HeapNode>{
  Vector<Integer> liveNode;// remaining nodes
  int lcost; // lower bound cost
  int level;// how many fixed
  // constructor
  public HeapNode(Vector<Integer> node,int lb, int lev){
   this.liveNode = new Vector<Integer>();
   this.liveNode.addAll(node);
   lcost = lb;
   level = lev;
  }

  @Override
  public int compareTo(HeapNode x) {// ordering by lcost
   return Integer.compare(this.lcost, x.lcost);
  }
  public boolean equals(Object x){
   if(!(x instanceof HeapNode)) return false;
   return lcost==((HeapNode)x).lcost;
  }

 }

 /**
  * compute a lower bound for remaining nodes (simple 2-min heuristic).
  *
  * @param liveNode remaining nodes (values are 1..n)
  * @param level current level (not strictly required by this LB)
  * @param cMatrix cost matrix indexed 1..n (cMatrix[0] unused)
  */
 public int computeLB(Vector<Integer> liveNode, int level, int[][] cMatrix)
 {
  if(liveNode == null || liveNode.size() == 0) return 0;
  final int INF = Integer.MAX_VALUE / 4;
  int sum = 0;
  int nIndexMax = cMatrix.length - 1; // assume cMatrix is (n+1) x (n+1)
  for(Integer u : liveNode){
   int min1 = INF, min2 = INF;
   for(int v = 1; v <= nIndexMax; v++){
    if(u == v) continue;
    int cost = cMatrix[u][v];
    if(cost == NoEdge) continue;
    if(cost < min1){
     min2 = min1;
     min1 = cost;
    } else if(cost < min2){
     min2 = cost;
    }
   }
   if(min1 == INF) {
    // no available outgoing edge -> infeasible, return a very large bound
    return Integer.MAX_VALUE/2;
   }
   if(min2 == INF) {
    // only one available edge, add only min1 (keeps bound admissible)
    sum += min1;
   } else {
    sum += (min1 + min2);
   }
  }
  // each edge counted twice in sum of two-min per vertex
  return sum / 2;
 }

 /**
  * Solve TSP with branch-and-bound (backtracking + LB pruning).
  *
  * @param cMatrix cost matrix indexed 1..n (cMatrix[0] unused)
  * @param n number of nodes
  */
 public int bb4TSP(int[][] cMatrix, int n)
 {
  // initial nodes 1..n
  Vector<Integer> allNodes = new Vector<Integer>();
  for(int i = 1; i <= n; i++) allNodes.add(i);

  // prepare visited array (1..n)
  boolean[] visited = new boolean[n+1];
  Vector<Integer> path = new Vector<Integer>();

  // choose start node as 1 (can be any)
  int start = 1;
  visited[start] = true;
  path.add(start);

  // build initial liveNode (unvisited)
  Vector<Integer> liveNode = new Vector<Integer>();
  for(int i = 1; i <= n; i++){
   if(i != start) liveNode.add(i);
  }

  // reset minCost
  minCost = Integer.MAX_VALUE;
  bestH.clear();

  // start DFS with pruning
  dfs(start, start, visited, liveNode, 1, 0, path, cMatrix, n);

  return minCost;
 }

 // recursive DFS with LB pruning
 private void dfs(int start, int current, boolean[] visited, Vector<Integer> liveNode,
                  int count, int currentCost, Vector<Integer> path, int[][] cMatrix, int n)
 {
  // if all visited, try to close tour
  if(count == n){
   int backCost = cMatrix[current][start];
   if(backCost != NoEdge){
    int total = currentCost + backCost;
    if(total < minCost){
     minCost = total;
     // record best path (closed tour)
     bestH.clear();
     bestH.addAll(path);
     bestH.add(start);
    }
   }
   return;
  }

  // compute a lower bound for remaining nodes
  int lb = computeLB(liveNode, count+1, cMatrix);
  if(lb == Integer.MAX_VALUE/2) return; // infeasible branch
  if(currentCost + lb >= minCost) return; // prune

  // try each candidate next node
  // iterate over a copy to allow modification
  Vector<Integer> candidates = new Vector<Integer>(liveNode);
  for(Integer next : candidates){
   int edgeCost = cMatrix[current][next];
   if(edgeCost == NoEdge) continue;
   // choose next
   visited[next] = true;
   path.add(next);
   // remove next from liveNode
   int idx = liveNode.indexOf(next);
   if(idx >= 0) liveNode.remove(idx);

   dfs(start, next, visited, liveNode, count+1, currentCost + edgeCost, path, cMatrix, n);

   // backtrack
   if(idx >= 0) liveNode.add(idx, next);
   path.remove(path.size()-1);
   visited[next] = false;
  }
 }

}
