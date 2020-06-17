open SharedType;
open Unit;

type state = {
    grid: array(array(unitType)),
    foundPath: bool
};

let initialState = {
    grid: {let base = Array.make_matrix(4, 8, Land(Road, Ordinary, NotPath))
            base[0][0] = Land(Road, Start, NotPath)
            base[3][7] = Land(Road, End, NotPath)
            base},
    foundPath: false
};

type action = 
    | Toggle(coord)
    | FindPath;

let gridReducer = (state, action) => {
    switch (action) {
        | Toggle(coord) => {
            state.grid[coord.row][coord.col] = unitChange(state.grid[coord.row][coord.col]);
            {grid: state.grid, foundPath: false};
        }
        | FindPath => {
            let path = AStar.aStar(state.grid);
            let _ = List.map((coord) => state.grid[coord.row][coord.col] = unitToPath(state.grid[coord.row][coord.col]), path);
            {grid: state.grid, foundPath: !state.foundPath};
        }
    }
};


[@react.component]
let make = () => {
    let (state, dispatch) = React.useReducer(gridReducer, initialState);

    <div className="grid">
        <button onClick=(_ => dispatch(FindPath))> 
            {
                switch (state.foundPath) {
                    | true => React.string("Clear Path")
                    | false => React.string("Find Path")
                };
            }
        </button> 
        {state.grid
        |> Array.mapi((rowId, row) => 
            Array.mapi((colId, _) => 
                <Unit 
                    key=("coord "++string_of_int(rowId)++" "++string_of_int(colId))
                    id=("unit "++string_of_int(rowId)++" "++string_of_int(colId))
                    action=Toggle({row: rowId, col: colId})
                    toggleUnit=dispatch
                    unit=state.grid[rowId][colId]
                    >
                </Unit>
            , row) 
            |> React.array)
        |> Array.mapi((rowId, row) =>
            <div 
                key=("row "++string_of_int(rowId))
                name=("row "++string_of_int(rowId))
                >
                {row}
            </div>
        )
        |> React.array;}
    </div>
};