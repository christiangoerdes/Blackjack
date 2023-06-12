import Card from "./Card";

export default function Player(player) {
    return (
        <div className="player">
            <span>Name: {player.name}</span>
            <span>Balance: {player.balance}</span>
            <span>Bet: {player.bet}</span>
            <span>In round: {player.in_round.toString()}</span>
            <span>Deck value: {player.deck_value}</span>
            {player.deck.map((card, index) => 
                <Card {...card} key={index} />
            )}
        </div>
    )
}