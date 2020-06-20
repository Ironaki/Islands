type coord = {
  row: int,
  col: int,
};

type initType =
  | Blank
  | Random
  | TokyoBay;

let coordStr = coordinate => string_of_int(coordinate.row) ++ " " ++ string_of_int(coordinate.col);
