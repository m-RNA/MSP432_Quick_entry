/**
 *  ======== tsort.js ========
 *  Package to support topological sorts: sort nodes in order of finishing
 *  time in DFS (lowest to highest)
 */
var tsort = {};
tsort.debug = console.log;
tsort.print = console.log;
tsort.debug = function (){};    /* comment out this line for trace debug */

/**
 *  ======== tsort.Node ========
 *  A Graph is a collection of Node's.  This is a Node constructor.
 *
 *  @param name name of this node
 *  @param graph the tsort.Graph that this node is a part of.
 */
tsort.Node = function(name, graph)
{
    this.name = name;       /* name of this node */
    this.graph = graph;     /* graph that this node belongs to */
    this.adj = [];          /* nodes directly reachable from this */
    this.inv_adj = [];      /* nodes that directly point to this */

    this.reset();           /* initialize the other fields of a Node */
};

/**
 *  ======== Node.pointsTo ========
 *  Establish a directed arc from this node to the specified node.
 *
 *  @param node the node that <this> node should be connected to
 *  @return void
 */
tsort.Node.prototype.pointsTo = function (node)
{
    if (this.adj[node.name] == null) {  /* avoid adding duplicate arcs */
        this.adj[node.name] = node;

        this.adj[this.adj.length] = node;
        node.inv_adj[node.inv_adj.length] = this;
    }
};

/**
 *  ======== Node.reset ========
 *  Initialize fields of Node that are modified during traversal.
 *
 *  @return void
 */
tsort.Node.prototype.reset = function ()
{
    this.color = "white";
    this.finish = 0;
    this.discovery = 0;
};

/**
 *  ======== Node.toString ========
 *  Convert this node to a string.
 *
 *  @return the name of this node
 */
tsort.Node.prototype.toString = function ()
{
    return (this.name);
};

/**
 *  ======== tsort.Graph ========
 *  Constructor for a Graph.
 *
 *  @param name the name of the graph
 */
tsort.Graph = function (name, error)
{
    this.name = name;       /* name of this graph */
    this.error = error;     /* error function to call */
    this.nodes = [];        /* array of nodes */
    this.mode = "forward";  /* mode is used to reverse the direction of arcs */
};

/**
 *  ======== Graph.reverse ========
 *  Reverse the direction of all arcs in the graph.  This allows one to
 *  easily determine reachability by reusing DFS traversal.
 *
 *  @return void
 */
tsort.Graph.prototype.reverse = function ()
{
    this.mode = (this.mode == "forward") ? "reverse" : "forward";
};

/**
 *  ======== Graph.toString ========
 *  Convert this graph to a string.
 *
 *  @return the name of this graph.
 */
tsort.Graph.prototype.toString = function ()
{
    return (this.name);
};

/**
 *  ======== Graph.mkNode ========
 *  Add a Node to this Graph.
 *
 *  @param name the name of the node to create and add to the graph
 *  @return the newly created node.
 */
tsort.Graph.prototype.mkNode = function (name)
{
    var node = this[name];
    if (node == null) {
        node = new tsort.Node(name, this);
        this.nodes[this.nodes.length] = node;
        this[name] = node;
    }
    return (node);
};

/**
 *  ======== Graph.DFS ========
 *  Do a DFS traversal of this Graph.
 *
 *  @param fxn function to call as each node is visited.
 *  @param start node to start from within the graph.
 */
tsort.Graph.prototype.DFS = function(fxn, start)
{
    var time = 0;

    tsort.debug(this + ".DFS("+ fxn.toString().split(" ")[1] +")");

    for (let i = 0; i < this.nodes.length; i++) {
        this.nodes[i].reset();
    }

    if (arguments.length < 2) {
        for (let i = 0; i < this.nodes.length; i++) {
            if (this.nodes[i].color == "white") {
                tsort.debug("DFSVisit starting from " + this.nodes[i] + " ...");
                DFSVisit(this.nodes[i], fxn);
            }
        }
    }
    else {
        tsort.debug("DFSVisit starting from " + start + " ...");
        DFSVisit(start, fxn);
    }

    /*
     *  ======== DFSVisit ========
     */
    function DFSVisit(node, fxn)
    {
        tsort.debug("DFSVisit(" + node + ", ...)");

        var adj = node.graph.mode == "forward" ? node.adj : node.inv_adj;

        node.color = "grey";
        node.discovery = ++time;

        /* recursively descend into adjacent nodes first */
        for (var i = 0; i < adj.length; i++) {
            if (adj[i].color == "white") {
                DFSVisit(adj[i], fxn);
            }
            else if (adj[i].color == "grey") {
                /* cycle detected: print it or "handle" it */
                var graph = adj[i].graph;
                var cycle = "";
                var prefix = "cycle detected in " + graph
                      + " starting at: " + adj[i];
                for (var j = 0; j < graph.nodes.length; j++) {
                    if (graph.nodes[j].color == "grey") {
                        cycle += graph.nodes[j] + " ";
                    }
                }
                if (graph.error != null) {
                    graph.error(adj[i], prefix, cycle);
                }
                else {
                    tsort.print(prefix + "\n    " + cycle);
                }
            }
        }

        /* all adj nodes have been visited, now visit node */
        node.color = "black";
        node.finish = ++time;
        fxn(node, time);
    }
};

/**
 *  ======== Graph.tsort ========
 *  Ordinary topological sort of the nodes.  Returns an array
 *  of all graph nodes in topological order.
 *
 *  @return an array of all graph nodes in topological order
 */
tsort.Graph.prototype.tsort = function ()
{
    var list = [];

    tsort.debug("tsort(" + this + ")");

    function append(finished) {
        list[list.length] = finished;
    }

    this.DFS(append);

    return (list);
};

/**
 *  ======== Graph.tsort2 ========
 *  Constrained topological sort of the nodes.  Returns an array
 *  of all graph nodes in topological order where nodes listed in
 *  lateList appear "as late as possible" and nodes that appear in
 *  the early list appear "as early as possible".
 *
 *  To make nodes appear as "early as possible" we simply start with
 *  these nodes in the DFS.
 *
 *  To make nodes appear as "late as possible" we first visit all nodes
 *  that can not reach the nodes in the lateList.
 *  <P>
 *  Note: when more than one node appears in the lateList, there is no
 *  well defined solution.  Thus, we apply a heuristic that treats the
 *  first node in the lateList as the "most important".
 *  <P>
 *  This is done by successively partitioning the set of nodes into
 *  groups that are ordered such that the nodes in the first group will
 *  be visited before the nodes in the second, etc.  We place nodes that
 *  can not reach "late as possible" nodes in earlier groups.
 *  <P>
 *  For example, the partitioning might proceed as follows:  <P>
 *      [{earliest nodes}, {remaining graph nodes}]  <P>
 *  <P>
 *      [{earliest nodes}, {can't reach lateList[0]}, {remaining nodes}]  <P>
 *  <P>
 *      [{earliest nodes}, {can't reach lateList[0] or lateList[1]},  <P>
 *          {can't reach lateList[0] and can reach lateList[1]}  <P>
 *          {remaining nodes}]
 *  <P>
 *  Once the partitioning is complete, we reorder the graph nodes in the
 *  order determined by the partitioning and simply call ordinary tsort
 *  (which starts with nodes that appear earlier in the node array first).
 *
 *  @param lateList array of nodes that should appear as "late as possible"
 *  @param earlyList array of nodes that should appear as "early as possible"
 *  @return an array of all graph nodes in topological order
 */
tsort.Graph.prototype.tsort2 = function (lateList, earlyList)
{
    /*
     *  ======== split ========
     *  Returns [{setA & setB}, {setA - setB}] if {setA & setB} is non-empty,
     *  otherwise split() returns null.
     */
    function split(setA, setB)
    {
        var count = 0;

        /* compute intersection (setA & setB) and difference (setA - setB) */
        var intersection = {};
        var difference = {};
        for (var i in setA) {
            if (setB[i] != null) {
                intersection[i] = setA[i];
                count++;
            }
            else {
                difference[i] = setA[i];
            }
        }

        /* if intersection is empty, return null */
        if (count == 0) {
            return (null);
        }

        /* return array with intersection and difference */
        return (new Array(intersection, difference));
    }

    /* pre-order graph nodes so lateList nodes appear "as late as possible" */

    var i, j;

    /* initialize partsArray to [{all graph nodes}] */
    var partsArray = [];
    set = {};
    for (i = 0; i < this.nodes.length; i++) {
        set[this.nodes[i].name] = this.nodes[i];
    }
    partsArray[0] = set;

    /* if necessary, split into [{early nodes}, {remaining graph nodes}] */
    if (arguments.length > 1 && earlyList.length != 0) {
        var set = {};
        for (let i = 0; i < earlyList.length; i++) {
            set[earlyList[i].name] = earlyList[i];
        }
        partsArray = split(partsArray[0], set);
    }

    /* for n in lateList, split first set that contains node that reaches n */
    for (let j = 0; j < lateList.length; j++) {
        var can = this.canReach(lateList[j]);

        /* find first set that contains a node that can reach lateList[j] */
        for (let i = 0; i < partsArray.length; i++) {
            /* split this set into two placing the unreachables first */
            var tmp = split(partsArray[i], can);
            if (tmp != null) {
                partsArray.splice(i, 1, tmp[1], tmp[0]);
                break;
            }
        }
    }

    /* copy nodes from ordered partition into graph's node array */
    for (j = i = 0; i < partsArray.length; i++) {
        for (var n in partsArray[i]) {
            this.nodes[j++] = (partsArray[i])[n];
        }
    }

    /* run ordinary tsort() on newly ordered nodes */
    return (this.tsort());
};

/**
 *  ======== Graph.canReach ========
 *  Returns an array of nodes that can reach the specified node.
 *
 *  @param node node to be reached
 *  @return an array of nodes that can reach <node>.
 */
tsort.Graph.prototype.canReach = function (node)
{
    var set = {};

    tsort.debug(this + ".canReach(" + node + ")");

    function append(finished) {
        set[finished.name] = finished;
    }

    this.reverse();             /* reverse the node arrows */
    this.DFS(append, node);     /* decend new graph */
    this.reverse();             /* restore node arrows */

    return (set);
};

/*
 *  ======== tsort.test ========
 */
tsort.test = function (verbose)
{
    var graph = new tsort.Graph("test");

    graph.mkNode("mem");
    graph.mkNode("clk");
    graph.mkNode("gbl");
    graph.mkNode("hwi");
    graph.clk.pointsTo(graph.gbl);
    graph.clk.pointsTo(graph.mem);
//    graph.gbl.pointsTo(graph.clk);

    tsort.print("Default order:   " + graph.tsort());

    if (verbose) {
        tsort.print("With MEM latest: " + graph.tsort2([graph.mem]));
        tsort.print("With GBL latest: " + graph.tsort2([graph.gbl]));
        tsort.print("With CLK latest: " + graph.tsort2([graph.clk]));
        tsort.print("With HWI latest: " + graph.tsort2([graph.hwi]));
        tsort.print("With all latest: "
            + graph.tsort2([graph.mem, graph.gbl, graph.clk]));

        tsort.print("With MEM early: "
            + graph.tsort2([], [graph.mem]));
        tsort.print("With HWI late and MEM early: "
            + graph.tsort2([graph.hwi], [graph.mem]));
    }
};

exports = tsort;
