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
    let minQueue = PriorityQueue.make((nodeA, nodeB) => nodeA.heuristic - nodeB.heuristic);
    PriorityQueue.push(minQueue, {
                                    heuristic: 0, 
                                    cost: 0,
                                    currCoord: startCoord,
                                    prevNode: None
                                });
    let rowLen = Array.length(grid);
    let colLen = Array.length(grid[0]);
    let visited: Hashtbl.t(string, unit) = Hashtbl.create(~random=true, colLen*rowLen);

    let getEuclidean = (coordinate) => abs(endCoord.row-coordinate.row)+abs(endCoord.col-coordinate.col);

    let getNeighbors = (coordinate: coord, visited) => {
        let right: coord = {...coordinate, col: coordinate.col+1};
        let bottom: coord = {...coordinate, row: coordinate.row-1};
        let left: coord = {...coordinate, col: coordinate.col-1};
        let upper: coord = {...coordinate, row: coordinate.row+1};
        let neighbors = [right, bottom, left, upper];
        List.filter((coordinate) => 
            coordinate.row >= 0 && coordinate.row < rowLen && coordinate.col >= 0 && coordinate.col < colLen
            && !Hashtbl.mem(visited, string_of_int(coordinate.row)++" "++string_of_int(coordinate.col))
            && grid[coordinate.row][coordinate.col] != Water,
            neighbors);
    };
    

    let rec aStarRecHelper = (~priorityQueue as pq: PriorityQueue.t(node), visited: Hashtbl.t(string, unit),) => {
        let minNode = PriorityQueue.pop(pq); // returns an Option

        switch (minNode) {
            | None => Unreacheable
            | Some(currNode) =>
                if (currNode.currCoord == endCoord) {
                    Last(currNode);
                } else {
                    if (!Hashtbl.mem(visited, string_of_int(currNode.currCoord.row)++" "++string_of_int(currNode.currCoord.col))){
                        Hashtbl.add(visited, string_of_int(currNode.currCoord.row)++" "++string_of_int(currNode.currCoord.col), ());
                    } 
                    let neighbors = getNeighbors(currNode.currCoord, visited);
                    let neighborNodes = List.map((neighbor) => 
                        {heuristic: currNode.cost+unitCost(grid[neighbor.row][neighbor.col])+getEuclidean(neighbor),
                        cost: currNode.cost+unitCost(grid[neighbor.row][neighbor.col]),         
                        currCoord: neighbor,
                        prevNode: Some(currNode)}, neighbors);
                    let _ = List.map((neighborNode) => PriorityQueue.push(pq, neighborNode), neighborNodes);
                    aStarRecHelper(~priorityQueue=pq, visited);
                }
        };
    };
    let lastNode = aStarRecHelper(~priorityQueue=minQueue, visited);
    Js.log(visited);
    let rec pathConstruct = (n: node) => {
        switch (n.prevNode) {
            | None => [n.currCoord]
            | Some(prevNode) => [n.currCoord, ...pathConstruct(prevNode)]
        }
    };
    
    switch (lastNode) {
        | Unreacheable => []
        | Last(n) => pathConstruct(n)
    };
 };


/* let aStar = (grid: array(array(unitType)), startCoord: coord, endCoord: coord) => {
    let rowLen = Array.length(grid);
    let colLen = Array.length(grid[0]);
    let minQueue = PriorityQueue.make((nodeA, nodeB) => nodeA.heuristic - nodeB.heuristic);
    PriorityQueue.push(minQueue, {
                                    heuristic: 0, 
                                    cost: 0,
                                    currCoord: startCoord,
                                    prevNode: None
                                });    

    let getNeighbors = (coordinate: coord, visited) => {
        let right: coord = {...coordinate, col: coordinate.col+1};
        let bottom: coord = {...coordinate, row: coordinate.row-1};
        let left: coord = {...coordinate, col: coordinate.col-1};
        let upper: coord = {...coordinate, row: coordinate.row+1};
        let neighbors = [right, bottom, left, upper];
        List.filter((coordinate) => 
            coordinate.row >= 0 && coordinate.row < rowLen && coordinate.col >= 0 && coordinate.col < colLen
            && !Hashtbl.mem(visited, string_of_int(coordinate.row)++" "++string_of_int(coordinate.col))
            && grid[coordinate.row][coordinate.col] != Water,
            neighbors);
    };

    let visited: Hashtbl.t(string, unit) = Hashtbl.create(~random=true, colLen*rowLen);
    let getEuclidean = (coordinate) => abs(endCoord.row-coordinate.row)+abs(endCoord.col-coordinate.col);                                
    
    while (true) {
        let minNode = PriorityQueue.pop(minQueue);
        switch (minNode) {
            | None => Unreacheable
            | Some(currNode) => {
                if (currNode.currCoord == endCoord) {
                    Last(currNode);
                } else {
                    if (!Hashtbl.mem(visited, string_of_int(currNode.currCoord.row)++" "++string_of_int(currNode.currCoord.col))){
                        Hashtbl.add(visited, string_of_int(currNode.currCoord.row)++" "++string_of_int(currNode.currCoord.col), ());
                    } 
                    let neighbors = getNeighbors(currNode.currCoord, visited);
                    let neighborNodes = List.map((neighbor) => 
                        {heuristic: currNode.cost+unitCost(grid[neighbor.row][neighbor.col])+getEuclidean(neighbor),
                        cost: currNode.cost+unitCost(grid[neighbor.row][neighbor.col]),         
                        currCoord: neighbor,
                        prevNode: Some(currNode)}, neighbors);
                    let _ = List.map((neighborNode) => PriorityQueue.push(minQueue, neighborNode), neighborNodes);
                }
            }
        }

        let rec pathConstruct = (n: node) => {
            switch (n.prevNode) {
                | None => [n.currCoord]
                | Some(prevNode) => [n.currCoord, ...pathConstruct(prevNode)]
            }
        };
        
        switch (lastNode) {
            | Unreacheable => []
            | Last(n) => pathConstruct(n)
        };
    }
}
 */