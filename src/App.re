[%bs.raw {|require('./App.css')|}];

[@react.component]
let make = () => {
  <main>
    <h2 className="title"> {React.string("Islands - A* Path Search")} </h2>
    <div className="side-bar">
      <p className="add-info">
        {React.string("Additional Info.")}
        <ul>
          <li>
            <a href="https://github.com/Ironaki/Islands#usage" target="_blank">
              {React.string("Instructions")}
            </a>
          </li>
          <li>
            <a href="https://github.com/Ironaki/Islands#algorithm" target="_blank"> {React.string("Algorithm")} </a>
          </li>
          <li>
            <a href="https://github.com/Ironaki/Islands#inspiration" target="_blank">
              {React.string("Inspiration")}
            </a>
          </li>
        </ul>
      </p>
    </div>
    <World />
    <div className="footnote">
      <p> {React.string("Built by Zixuan Li with ReasonML and ReasonReact. June 2020.")} </p>
      <a href="https://github.com/Ironaki" target="_blank"> {React.string("GitHub")} </a>
    </div>
  </main>;
};
