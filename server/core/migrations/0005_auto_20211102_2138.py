# Generated by Django 3.1.4 on 2021-11-02 20:38

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('core', '0004_auto_20211018_1737'),
    ]

    operations = [
        migrations.AddField(
            model_name='game',
            name='game_over',
            field=models.BooleanField(default=False),
        ),
        migrations.AddField(
            model_name='game',
            name='winner',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='won_set', to='core.player'),
        ),
    ]