# Binary Tree:

* methematische Definition (rekursiv) [1, S. 308ff]
    * endliche Menge von Knoten
    * spezieller Knoten root $\Rightarrow$ steht an der Spitze des Baums
    * restliche Knoten in ($hier: m = 2$) disjunkte Mengen geteilt $T_1, T_2, ..., T_m =: u, T_{m+1}, ..., T_n =: v$
    * diese Mengen sind Unterbäume (subtrees)
    * Schaubild Unterbäume mit Keilen
    * $degree(n) = k$ (Anzahl Unterbäume k des Knotens n)
    * Abbruchbedingung $\Rightarrow$ Unterbaum hat nur root $\Rightarrow$ Blatt (leaf)
    * leaf $\Rightarrow$ Unterbaum mit nur einem Knoten
    * $d(n) = 1 \Leftrightarrow n = leaf$

* Höhe eines vollen Binärbaums ~ $ log_2(n)$ [3, S. 40]

## Terminologie:
* Knoten sind roots von Unterbäumen
* Knoten haben Kinder $m$ (children) (maximal 2 bei Binärbaum)
* Root (Wurzel) parent von subtree (Unterbaum)
* leaves haben keine Kinder $\Rightarrow$ Länge 1

## mathematische Notation:
Binary Tree: $t = (u, r, v), u, v$ sind Binary Trees, $r \in K$ ($K$ Schlüsselmenge z.B. ($\mathbb{Z}, \mathbb{N}$))

Sei $n \in \mathbb{N}$
$T = \{ t_1, t_2, ..., t_n \} $ mit $ \: t_i \ne t_j \: $ für $i \ne j$

$root : P(T) \rightarrow T$
$r_i = root(T)$ für $i \le n$

Linker Unterbaum $u = \{ t \in T \: | \: t < r_i \}$
Rechter Unterbaum $v = \{ t \in T \: | \: t > r_i \}$

## Sorted Binary Tree (Binärer Suchbaum):

* Sortierung der Knoten nach dem key [2, S. 148]
* Sortierungskriterium kann nach Anwendung unterschiedlich sein [3, S. 37]
* In-Order Traversel gibt keys in geordneter Reihenfolge aus
    $\Rightarrow$ Linker Unterbaum: alle Elemente kleiner als root
    $\Rightarrow$ Rechter Unterbaum: alle Elemente größer als root

## In-order traversel: [1, S. 319]
    - Leere Baum: tue nichts
    - Besuche den Linken Unterbaum
    - Besuche die Wurzel
    - Besuche den Rechten Unterbaum

Grober Abriss Python
```python
class BinTree:
    left: BinTree
    right: BinTree
    root: int
```

Naive Rekursive Implementierung (langsam)
```python
def inorder(t):
    if t == None:
        return

    inorder(t.left)
    visit(t.root)       # make computation with node
    inorder(v.right)
```

Flotte Iterative Implementierung mit Stack frei nach Knuth [1, S. 320]
```python
    def inorder(t):
        s = []
        node = t.root

        while node != None or len(s) != 0:
            if node != None:
                s += [node]
                node = n.left
            else:
                node = s.pop()
                visit(node)
                node = node.right
```

$\Rightarrow$ Kleine Demo in python

## Self Balancing Binary Tree:

* balanced trees $\Rightarrow$ Länge jedes Pfades unterscheidet sich um nicht mehr als 1
* spezielle Definition für Algorithmen und Datenstruckturen
* Kein balancierter Binärbaum [Proof](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#/media/File:5_minimal_red-black_trees_nN.svg)

## Einordung Red-Black-Trees

* spezieller Weg des Balancierens
* nur Binär $\Rightarrow$ für allgemeine Bäume B-Tree, AVL-Tree

## Implemetierung Binärbaume
* struct mit Pointer zu children $(l, r)$

Quellen:
    [1] The Art of Computer Programming
    [2] Algorithms and Datastructures
    [3] Algorithms in C