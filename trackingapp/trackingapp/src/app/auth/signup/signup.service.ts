import { Injectable } from '@angular/core';
import { Person } from 'src/app/data-models/Person';

let incrementID = 0;
let currentSession = 0; // usuario actual

@Injectable({
  providedIn: 'root'
})

export class SignupService {
  persons: Array<Person> = [
    // alumno de prueba
    {
      personId: 0,
      fullname: 'Test usuario',
      email: 'prueba@test.com',
      terrainName: 'rancho genérico',
      country: 'Mexico',
      password: '12345',
      twitterAt: 'test',
      ownedAnimals: '',
      terrainNodes: ''
    }
  ];

  constructor() { }

  generateID(): number {
    incrementID++;
    return incrementID;
  }

  createPerson(objct) {
    objct.id = this.generateID();
    // objct.rol = 1; para admin
    console.log(objct);
    this.savePerson(objct);
    this.persons.push(objct);
  }

  savePerson(objct) {
    console.log('Se esta guadando el obj con id', objct, objct.id);
    // this.logUser(objct.id);
    // invocar guardado en el back-end
  }

  // Invocar logUser DESPUÉS del la comprobación de screenname y contraseña al presionar login, no al crear cuenta.
  logUser(id: number) {
    console.log('Iniciando sesión como persona ', id);
    currentSession = id;
  }

  isLoggedIn(): boolean {
    if (currentSession) {
      return true;
    }
    return false;
  }

  logoutUser() {
    console.log('Cerrando sesión de persona ', currentSession);
    currentSession = 0;
  }

  getCurrentPerson(): Person {
    console.log('Current user logged: ', currentSession);
    console.log(this.getPerson(currentSession));
    return this.getPerson(currentSession);
  }

  getPerson(id: number): Person {
    return this.persons[id];
  }

  getPersons(): Array<Person> {
    console.log(this.persons);
    return this.persons.slice();
  }


}
