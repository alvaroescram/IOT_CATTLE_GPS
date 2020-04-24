import { Component, OnInit, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'app-header',
  templateUrl: './header.component.html',
  styleUrls: ['./header.component.css']
})
export class HeaderComponent implements OnInit {

  @Output() Toggle = new EventEmitter();

  constructor() { }

  ngOnInit() {
  }

  onToggleSidenav() {

    this.Toggle.emit();

  }
}
