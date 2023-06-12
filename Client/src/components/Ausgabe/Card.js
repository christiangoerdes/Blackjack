export default function Card(card) {
    return (
        <div className="card">
            <span>Suit: {card.suit} --- Type: {card.type}</span>
        </div>
    )
}