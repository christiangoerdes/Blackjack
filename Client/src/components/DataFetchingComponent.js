import { useState, useEffect } from "react";
import api from "../api/axiosConfig";

export default function DataFetchingComponent() {
    const [data, setData] = useState(null);

    const fetchData = async () => {
        try {
            const response = await api.get("/")
            setData(response.data)
        }
        catch (err) {
            console.log(err)
        }
    }

    useEffect(() => {
        const interval = setInterval(fetchData, 1000)

        return () => {
            clearInterval(interval);
        }
    })
    
    return (
        <>
            <h2>Polling Data</h2>
            {data ? data.gameState : <p>Lade Daten...</p>}
        </>
    )
}
