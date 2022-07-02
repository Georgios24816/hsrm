package de.hsrm.ads;

import java.util.Iterator;
import java.util.LinkedList;

public class BinTree implements Iterable<Integer> {

	Node root;
	TreeIteratorLevelOrder it;

	static class Node {

		Node left;
		Node right;
		int value;

		Node(int value) {
			this.value = value;
		}

		Node(int value, Node left, Node right) {
			this.value = value;
			this.left = left;
			this.right = right;
		}
		
	}
	
	
	class TreeIteratorLevelOrder implements Iterator<Integer> {
    		Node current;
		boolean left;
		
		TreeIteratorLevelOrder(Node n) {
			current = n;
			left = true;
		}

		public boolean hasNext() {
			/*
			if (left) {
			  left = false;
			  return current.left == null;
			} else {
			  left = true;
			  return current.right == null;
			}
			*/
			
			return current != null;
		}

		public Integer next() {
			// FIXME
		}

	}

	public Iterator<Integer> iterator() {
		return new TreeIteratorLevelOrder(root);
	}
	
		
	
	public BinTree() {
		root = null;
		it = new TreeIteratorLevelOrder(root);
	}

	public BinTree(Node root) {
		this.root = root;
		it = new TreeIteratorLevelOrder(root);
	}

	public int height() {
		int h = 0;
		while (it.hasNext()) {
			it.next();
			h++;
		}
		
		return (int) (Math.log(h) / Math.log(2));
	}
	
	public String pathToMax() {
		// FIXME
	}
	
	public static void main(String[] args) {
	}

}

