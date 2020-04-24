import { Animal } from './Animal';
import { TerrainNode } from './TerrainNode';

export class Person {
    constructor(
        personId: number,
        fullname: string,
        email: string,
        terrainName: string,
        country: string,
        password: string,
        twitterAt?: string,
        ownedAnimals?: Array<Animal>,       // lista de animales
        terrainNodes?: Array<TerrainNode>   // lista de nodos*/
    ) { }
}
