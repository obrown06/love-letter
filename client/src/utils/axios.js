import axios from 'axios';
import { checkAuthenticated } from 'utils/auth.js';

const myaxios = axios.create({ withCredentials: true });

const apiEndpoint = "ghoulish-mansion-08890.herokuapp.com";

export { myaxios, apiEndpoint };
