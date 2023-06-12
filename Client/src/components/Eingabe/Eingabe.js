import { useEffect, useState } from "react";
import api from "../../api/axiosConfig";
import "./Eingabe.css";

export default function Eingabe() {
    const [name, setName] = useState("");
    const [inGame, setInGame] = useState(false);
    const [betPlaced, setBetPlaced] = useState(false);
    const [bet, setBet] = useState(0);

    const join = () => {
        api
        .get(`/join?name=${name}&password=${"password"}`)
        .then((response) => response.data.success ? setInGame(true) : console.log("Cant join now!"))
    }
    const leave = () => {
        api
        .get(`/leave?name=${name}&password=${"password"}`)
        .then((response) => response.data.success ? setInGame(false) : console.log("Cant leave now!"))
    }
    const startRound = () => {
        api
        .get("/start_round")
        .then((response) => response.data.success ? setBetPlaced(false) : console.log("Cant start round now!"));
    }
    const placeBet = () => {
        api
        .get(`/place_bet?name=${name}&password=password&bet=${bet}`)
        .then((response) => response.data.success ? setBetPlaced(true) : console.log("Cant place bet now"))
    }
    const draw = () => {
        api
        .get(`/draw?name=${name}&password=password`)
    }
    const skip = () => {
        api
        .get(`/skip?name=${name}&password=password`)
    }

    return (
        <>
            {!inGame && <div className="username">
                <label htmlFor="name">Name</label>
                <input
                    type="text"
                    id="name"
                    value={name}
                    onChange={e => setName(e.target.value)} 
                />
            </div>}
            {inGame && !betPlaced && <div className="username">
                <label htmlFor="bet">Bet</label>
                <input
                    type="number"
                    id="bet"
                    min={5}
                    max={100}
                    value={bet}
                    onChange={e => setBet(e.target.value)}
                />
            </div>}
            <div className="eingabe">
                <button
                    onClick={join}
                    disabled={name.length === 0}
                >
                    Join
                </button>
                <button
                    onClick={leave}
                >
                    Leave
                </button>
                <button
                    onClick={startRound}
                >
                    Start Round
                </button>
                <button
                    onClick={placeBet}
                >
                    Place Bet
                </button>
                <button
                    onClick={draw}
                >
                    Draw
                </button>
                <button
                    onClick={skip}
                >
                    Skip
                </button>
            </div>
        </>
    )
}