import { Component, OnInit } from '@angular/core';
import { SignupService } from '../signup/signup.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {

  constructor(private signupService: SignupService,
              private router: Router ) { }

  ngOnInit(): void {
  }

  loginPerson(events?) {
    // iniciar sesión
    // logUser( /*id*/);
    this.router.navigate(['']);
  }
}
