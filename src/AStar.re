open Unit;
open SharedType;

type node = {
  heuristic: int, // real cost+heuristic
  cost: int, // real cost
  currCoord: coord,
  prevNode: option(node) // Link to previous node for path reconstruction, None for starting node
};

type result =
  | Last(node)
  | Unreacheable;

let aStar = (grid: array(array(unitType)), startCoord: coord, endCoord: coord) => {
  /*
   A* Algorithm

   Select min f(coord) in the priority queue for search, where
   f(coord) = g(coord)+h(coord)
   g(coord): actual cost from startCoord to coord
   h(coord): heuristic estimate from coord to endCoord (Manhattan distance assuming each step cost 1)
    */
  let rowLen = Array.length(grid);
  let colLen = Array.length(grid[0]);

  /* Utility functions */
  let getEuclidean = coordinate => abs(endCoord.row - coordinate.row) + abs(endCoord.col - coordinate.col);

  let getNeighbors = (coordinate, currCost, visited) => {
    let right = {...coordinate, col: coordinate.col + 1};
    let bottom = {...coordinate, row: coordinate.row - 1};
    let left = {...coordinate, col: coordinate.col - 1};
    let upper = {...coordinate, row: coordinate.row + 1};

    [right, bottom, left, upper]
    |> List.filter(neiCoord =>
         neiCoord.row >= 0
         && neiCoord.row < rowLen
         && neiCoord.col >= 0
         && neiCoord.col < colLen
         && !isWater(grid[neiCoord.row][neiCoord.col])
         && (
           !visited->Hashtbl.mem(coordStr(neiCoord))
           || currCost
           + unitCost(grid[neiCoord.row][neiCoord.col]) < visited->Hashtbl.find(coordStr(neiCoord))
         )
       );
  };

  // pq: pop min(g(coord)+h(coord))
  let minQueue = PriorityQueue.make((nodeA, nodeB) => nodeA.heuristic - nodeB.heuristic);
  minQueue->PriorityQueue.push({
    heuristic: getEuclidean(startCoord),
    cost: 0,
    currCoord: startCoord,
    prevNode: None,
  });

  // visited map: coordinate: g(coordinate)
  let visited: Hashtbl.t(string, int) = Hashtbl.create(~random=true, colLen * rowLen);
  visited->Hashtbl.add(coordStr(startCoord), 0);

  /* Main A* Algorithm.
     P.S. this recursive function transpile to loop in JavaScript */
  let rec aStarRec = (~priorityQueue as pq: PriorityQueue.t(node), visited: Hashtbl.t(string, int)) => {
    let minNode = PriorityQueue.pop(pq); // returns an Option
    switch (minNode) {
    | None => Unreacheable
    | Some(currNode) =>
      if (currNode.currCoord == endCoord) {
        Last(currNode);
      } else {
        let {cost: currCost, currCoord} = currNode;
        let neighbors = getNeighbors(currCoord, currCost, visited);
        let _ =
          neighbors |> List.map(n => visited->Hashtbl.replace(coordStr(n), currCost + unitCost(grid[n.row][n.col])));

        let neighborNodes =
          neighbors
          |> List.map(n =>
               {
                 heuristic: currCost + unitCost(grid[n.row][n.col]) + getEuclidean(n),
                 cost: currCost + unitCost(grid[n.row][n.col]),
                 currCoord: n,
                 prevNode: Some(currNode),
               }
             );
        let _ = neighborNodes |> List.map(nn => pq->PriorityQueue.push(nn));
        aStarRec(~priorityQueue=pq, visited);
      }
    };
  };
  let lastNode = aStarRec(~priorityQueue=minQueue, visited);

  /* Path Construction */
  let rec pathConstruct = (n: node) => {
    switch (n.prevNode) {
    | None => [n.currCoord]
    | Some(prevNode) => [n.currCoord, ...pathConstruct(prevNode)]
    };
  };

  switch (lastNode) {
  | Unreacheable => []
  | Last(n) => pathConstruct(n)
  };
};
