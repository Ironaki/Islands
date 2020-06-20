open Unit;
open SharedType;

type node = {
  heuristic: int,
  cost: int,
  currCoord: coord,
  prevNode: option(node),
};

type result =
  | Last(node)
  | Unreacheable;

let aStar = (grid: array(array(unitType)), startCoord: coord, endCoord: coord) => {
  /*
   A* Algorithm

   priority: f(n) = g(n)+h(n)
   */
  let rowLen = Array.length(grid);
  let colLen = Array.length(grid[0]);

  let minQueue = PriorityQueue.make((nodeA, nodeB) => nodeA.heuristic - nodeB.heuristic);
  PriorityQueue.push(minQueue, {heuristic: 0, cost: 0, currCoord: startCoord, prevNode: None});

  let coordStr = coordinate => string_of_int(coordinate.row) ++ " " ++ string_of_int(coordinate.col);

  // visited map: coordinate -> g(coord)
  let visited: Hashtbl.t(string, int) = Hashtbl.create(~random=true, colLen * rowLen);
  visited->Hashtbl.add(coordStr(startCoord), 0);

  let getEuclidean = coordinate => abs(endCoord.row - coordinate.row) + abs(endCoord.col - coordinate.col);

  let getNeighbors = (coordinate, currCost, visited) => {
    let right = {...coordinate, col: coordinate.col + 1};
    let bottom = {...coordinate, row: coordinate.row - 1};
    let left = {...coordinate, col: coordinate.col - 1};
    let upper = {...coordinate, row: coordinate.row + 1};
    let neighbors = [right, bottom, left, upper];
    let validN =
      List.filter(
        coordinate =>
          coordinate.row >= 0
          && coordinate.row < rowLen
          && coordinate.col >= 0
          && coordinate.col < colLen
          && (
            !visited->Hashtbl.mem(coordStr(coordinate))
            || currCost
            + unitCost(grid[coordinate.row][coordinate.col]) < visited->Hashtbl.find(coordStr(coordinate))
          )
          && grid[coordinate.row][coordinate.col] != Water,
        neighbors,
      );
    validN;
  };

  let rec aStarRecHelper = (~priorityQueue as pq: PriorityQueue.t(node), visited: Hashtbl.t(string, int), count) => {
    let minNode = PriorityQueue.pop(pq); // returns an Option
    switch (minNode) {
    | None => Unreacheable
    | Some(currNode) =>
      if (currNode.currCoord == endCoord) {
        Js.log(count);
        Last(currNode);
      } else {
        let neighbors = getNeighbors(currNode.currCoord, currNode.cost, visited);
        let _ = List.map(n => visited->Hashtbl.replace(coordStr(n), currNode.cost + unitCost(grid[n.row][n.col])), neighbors);

        let neighborNodes =
          List.map(
            neighbor =>
              {
                heuristic: currNode.cost + unitCost(grid[neighbor.row][neighbor.col]) + getEuclidean(neighbor),
                cost: currNode.cost + unitCost(grid[neighbor.row][neighbor.col]),
                currCoord: neighbor,
                prevNode: Some(currNode),
              },
            neighbors,
          );
        let _ = List.map(neighborNode => PriorityQueue.push(pq, neighborNode), neighborNodes);
        aStarRecHelper(~priorityQueue=pq, visited, count + 1);
      }
    };
  };
  let lastNode = aStarRecHelper(~priorityQueue=minQueue, visited, 0);
  Js.log(visited);
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