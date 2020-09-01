import axios from 'axios';
import { checkAuthenticated } from 'utils/auth.js';

const myaxios = axios.create({});

export { myaxios };
