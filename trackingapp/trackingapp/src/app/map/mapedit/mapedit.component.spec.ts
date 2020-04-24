import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { MapeditComponent } from './mapedit.component';

describe('MapeditComponent', () => {
  let component: MapeditComponent;
  let fixture: ComponentFixture<MapeditComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ MapeditComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(MapeditComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
