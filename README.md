Link GitLab = https://gitlab.cs.pub.ro/adrian_eduard.negru/Tema1SO


Negru Adrian Eduard 332CC

*** Hash Table ***


	Implementarea unui hash table printr-un vector de liste. Fiecare element din vector 
va retine un pointer catre inceputul listei. Nodul din lista contine cuvantul introdus
si un pointer (next) catre urmatorul nod. Un nod care este ultimul element din lista va
avea next = NULL. Un hash table va retine si o dimensiune a vectorului de bucket-uri. 
	
	1) Add
	
	Adauga un element in hash table. Cauta daca elementul exista pe pozitia indicata de 
functia de hash. Daca elementul nu exista este adaugat la lista.

	2) Remove
	
	Sterge elementul dorit (daca exista) din lista indicata de functia de hash.
	
	3) Find
	
	Cauta in hash table o anumita valoare prin parcurgerea listei de la indexul indicat
de functia de hash.

	4) Print bucket
	
	Afiseaza lista de la indexul indicat prin parcurgerea acesteia pana la ultimul element.
	
	5) Print
	
	Afiseaza toate valorile din hash table. Parcurg secvential fiecare lista de la fiecare index al
vectorului de bucket-uri din hash table.

	6) Resize
	
	Modific dimensiunea hash table-ului. Creez un nou hashtable si reintroduc valorile din vechiul 
hash table in noul hash table (functia de hash va returna valori diferite cand se modifica dimensiunea).
Sterg valorile din vechiul hash table si fac astfel incat fiecare element din vectorul de bucket-uri sa 
pointeze catre noul hashtable.

	7) Clear 
	
	Sterge toate valorile din hash table 
	
	
	
	
	
	

