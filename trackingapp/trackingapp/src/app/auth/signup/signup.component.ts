import { Component, OnInit } from '@angular/core';
import { Person } from 'src/app/data-models/Person';
import { SignupService } from './signup.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-signup',
  templateUrl: './signup.component.html',
  styleUrls: ['./signup.component.css']
})
export class SignupComponent implements OnInit {
  persons: Array<any> = [];
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

  constructor(private signupService: SignupService,
              private router: Router ) { }

  ngOnInit(): void {
  }

  addPerson(events?) {
    if (!this.persona) { return; }
    this.signupService.createPerson(this.persona);
    this.router.navigate(['./signin']);
  }

}
