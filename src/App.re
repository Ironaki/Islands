[%bs.raw {|require('./App.css')|}];

[@react.component]
let make = () => {
  <main>
    <h2 className="title"> {React.string("Islands - A* Path Search")} </h2>
    <World />
    <div className="footnote">
      <p> {React.string("Built by Zixuan Li with ReasonML and ReasonReact. June 2020.")} </p>
      <a href="https://github.com/Ironaki" target="_blank"> {React.string("GitHub")} </a>
    </div>
  </main>;
};
