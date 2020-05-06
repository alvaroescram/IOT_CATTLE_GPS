import { Component, OnInit } from '@angular/core';
import { SignupService } from '../auth/signup/signup.service';
import { Person } from '../data-models/Person';

@Component({
  selector: 'app-profile',
  templateUrl: './profile.component.html',
  styleUrls: ['./profile.component.css']
})
export class ProfileComponent implements OnInit {

  // Persona para sesión
  persona: Person = {
    personId: '',
    fullname: '',
    email: '',
    terrainName: '',
    country: '',
    password: '',
    twitterAt: '',
    ownedAnimals: '',
    terrainNodes: ''
  };

  constructor(private signupService: SignupService  ) { }

  ngOnInit(): void {
    this.persona = this.signupService.getCurrentPerson();
    console.log(this.persona);
  }

}
