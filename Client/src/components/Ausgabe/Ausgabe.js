import { useEffect, useState } from "react";
import api from "../../api/axiosConfig";
import "./Ausgabe.css";

import Card from "./Card";
import Player from "./Player";

export default function Ausgabe() {
    const [gameState, setGameState] = useState({
        dealer : {
            deck : []
        },
        players : ""
    });
    
    /* const getGameState = () => {
        api.get("/").then((response) => setGameState(response.data))
    }
    const logGameState = () => {
        console.log(gameState);
    } */

    const getGameState = async () => {
        try {
            const response = await api.get("/");
            setGameState(response.data);
        }
        catch(err) {
            console.log(err);
        }
    }

    useEffect(() => {
        const interval = setInterval(getGameState, 500);

        return () => {
            clearInterval(interval);
        }
    })

    return (
        <div className="ausgabe">
            <div className="game-info">
                <h2>Game Info</h2>
                <span>Game State: {gameState.gameState}</span>
                <span>Initial Balance: {gameState.initBalance}</span>
                <span>Minimum Bet: {gameState.minBet}</span>
                <span>Turn: {gameState.players.length !== 0
                && gameState.turn < gameState.players.length
                && gameState.players[gameState.turn].name}</span>
            </div>
            <div className="player-info">
                <h2>Players</h2>
                {/* <Card {...testCard} /> */}
                <div className="players">
                    {gameState.players.length !== 0 && gameState.players.map((player, index) => 
                        <Player {...player} key={index} />
                    )}
                </div>
            </div>
            <div className="dealer-info">
                <h2>Dealer</h2>
                <span>Deck value: {gameState.dealer.deck.length !== 0 && gameState.dealer.deck_value}</span>
                {gameState.dealer.deck.length !== 0 && gameState.dealer.deck.map((card, index) => 
                    <Card {...card} key={index} />
                )}
            </div>
        </div>
    )
}