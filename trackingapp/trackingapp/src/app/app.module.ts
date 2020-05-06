import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponent } from './auth/login/login.component';
import { ProfileComponent } from './profile/profile.component';
import { GraphsComponent } from './graphs/graphs.component';
import { MapComponent } from './map/map.component';
import { HeaderComponent } from './nav/header/header.component';
import { SidenavListComponent } from './nav/sidenav-list/sidenav-list.component';
import { TabsComponent } from './nav/tabs/tabs.component';
import { SignupComponent } from './auth/signup/signup.component';
import { MapeditComponent } from './map/mapedit/mapedit.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { HomeComponent } from './home/home.component';
import { FlexLayoutModule } from '@angular/flex-layout';
import { FormsModule } from '@angular/forms';
import { MatGridListModule } from '@angular/material/grid-list';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatTabsModule } from '@angular/material/tabs';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatIconModule } from '@angular/material/icon';

@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    ProfileComponent,
    GraphsComponent,
    MapComponent,
    HeaderComponent,
    SidenavListComponent,
    TabsComponent,
    SignupComponent,
    MapeditComponent,
    HomeComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    BrowserAnimationsModule,
    FlexLayoutModule,
    FormsModule,
    MatGridListModule,
    MatSidenavModule,
    MatTabsModule,
    MatToolbarModule,
    MatIconModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
