public void add(T obj, int priority)
{
	Node newNode = new Node();  
    newNode.obj = obj;  
    newNode.priority = priority;  
    
    // List ist noch leer und bekommt das 1. Element
    if (head == null) {  
        head = newNode;  
        tail = newNode;  
        newNode.next = null;  
    }

    else {  
        if (priority <= head.priority) {  
            newNode.next = head;  
            head.prev = newNode.next;  
            head = newNode;  
        }  
    
        else if (priority > (tail).priority) {  
            newNode.next = null;  
            tail.next = newNode;  
            newNode.prev = tail.next;  
            tail = newNode;  
        }  
    
        else {  
    		//richtige Position zum Einfügen finden
            Node start = head.next;  
            while (start.priority > priority)   
                start = start.next;              
            start.prev.next = newNode;  
            newNode.next = start.prev;  
            newNode.prev = start.prev.next;  
            start.prev = newNode.next;  
        }  
    }  
    headont = head; 
    rear = tail; 
}  

public T pop()
{
	Node temp = head;  
    T obj = temp.info;  
    head = head.next;  
    
	if (head == null)   
       tail = null;  
      
    front = head; 
    rear = tail; 

    return obj;  
}

public void neteOne(Node n)
{
		if (head == null || n == null) 
            return null; 
  
        if (head == n) 
            head = n.next; 
  
        if (n.next != null) 
            n.next.prev = n.prev; 
  
        if (n.prev != null) 
            n.prev.next = n.next; 
  
        n = null; 
  
        return head; 
}

public void removeAll(int priority)
{
 		if (head == null) 
            return null; 
  
        Node current = head; 
        Node next; 
  
        //Liste durchlaufen 
        while (current != null) 
        { 
            // passende Node gefunden
            if (current.priority == priority) 
            { 
                next = current.next; 
                head = neteNode(head, current); 
                current = next; 
            } 
  
            //weiter nach passenden Nodes suchen
            else
                current = current.next; 
  
        } 
}

