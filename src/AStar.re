open Unit;
open Grid;

type node = {
    heuristic: int,
    cost: int,
    currCoord: coord,
    prevCoord: coord,
};

type result =
    | Last(node)
    | Unreacheable;



let aStar = (grid: array(array(unitType))) => {
    let minQueue = PriorityQueue.make((nodeA, nodeB) => nodeA.heuristic - nodeB.heuristic);
    PriorityQueue.push(minQueue, {
                                    heuristic: 0, 
                                    cost: 0,
                                    currCoord: {row: 0, col: 0,}, 
                                    prevCoord: {row: -1, col: -1,}
                                });
    let rowLen = Array.length(grid);
    let colLen = Array.length(grid[0]);
    let visited = Hashtbl.create(rowLen * colLen);

    let getEuclidean = (coordinate) => abs(rowLen-coordinate.row)+abs(colLen-coordinate.col);

    let getNeighbors = (coordinate: coord, visited) => {
        let right: coord = {...coordinate, col: coordinate.col+1};
        let bottom: coord = {...coordinate, row: coordinate.row-1};
        let left: coord = {...coordinate, col: coordinate.col-1};
        let upper: coord = {...coordinate, row: coordinate.row+1};
        let neighbors = [right, bottom, left, upper];
        List.filter((coordinate) => 
            coordinate.row >= 0 && coordinate.row < rowLen && coordinate.col >= 0 && coordinate.col < colLen
            && !Hashtbl.mem(visited, coordinate)
            && grid[coordinate.row][coordinate.col] != Water,
            neighbors);
    };

    

    let rec aStarRecHelper = (~priorityQueue as pq: PriorityQueue.t(node), visited: Hashtbl.t(coord, unit),) => {
        let minNode = PriorityQueue.pop(pq); // returns an Option
        switch (minNode) {
        | None => Unreacheable
        | Some(currNode) =>
        if (currNode.currCoord == {row: rowLen - 1, col: colLen - 1}) {
            Last(currNode);
        } else {
            Hashtbl.add(visited, currNode.currCoord, ());
            let neighbors = getNeighbors(currNode.currCoord, visited);
            let neighborNodes = List.map((neighbor) => 
                {heuristic: currNode.cost+unitCost(grid[neighbor.row][neighbor.col])+getEuclidean(neighbor),
                cost: currNode.cost+unitCost(grid[neighbor.row][neighbor.col]),
                currCoord: neighbor,
                 prevCoord: currNode.currCoord}, neighbors);
            let _ = List.map((neighborNode) => PriorityQueue.push(pq, neighborNode), neighborNodes);
            aStarRecHelper(~priorityQueue=pq, visited);
        }
        };
    };
    aStarRecHelper(~priorityQueue=minQueue, visited);
};
