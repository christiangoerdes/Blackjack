import axios from "axios";

export default axios.create({
    baseURL: 'https://98e6-2a01-598-d833-180f-a81c-4004-fc7a-74e8.ngrok-free.app',
    headers: {"ngrok-skip-browser-warning": "true"}
});
